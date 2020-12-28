let tail = s => String.sub(s, 1, String.length(s) - 1);

let fxi = (fx, list) =>
  list
  |> List.mapi((idx, a) => fx(string_of_int(idx), a))
  |> Array.of_list
  |> React.array;

let isSpecialChar = ch =>
  switch (ch) {
  | '!'..'/' => true
  | ':'..'@' => true
  | '['..'`' => true
  | '{'..'~' => true
  | _ => false
  };

let rec repetitiveChecker = (s, lastChar) =>
  switch (lastChar, s) {
  | (_, "") => true
  | (None, s) => repetitiveChecker(tail(s), Some(s.[0]))
  | (Some(c), s) =>
    let hd = s.[0];
    let tl = tail(s);
    c == hd ? false : repetitiveChecker(tl, Some(hd));
  };

/** Check if this string has emoji or not. */
let isHasEmoji = s =>
  s
  |> Rope.str_to_list
  |> Rope.rev_utf16_be
  |> Emo.emoji_folder
  |> List.exists(x => x != []);

let emoPointer = (s: string) =>
  s
  |> Rope.str_to_list
  |> Rope.rev_utf16_be
  |> Emo.emoji_folder
  |> List.exists(
       fun
       | [0x1F448 | 0x1F449]
       | [0x1F448 | 0x1F449, 0x1F3FB | 0x1F3FC | 0x1F3FD | 0x1F3FE | 0x1F3FF] =>
         true
       | _ => false,
     );

/** Unified password field state. */
type state = {
  content: string, // password content
  respected: option(bool), // If None, do not check for keydown
  passed: list(string), // passed rules' text
  failed: option(string), // current failed rule
  showed: bool, // is this field currently plain text
  iteration: int, // used to assign unique index for each error message, prevent unnecessary repaint
  disabled: bool, // For some special circumstance, this input field is disabled
  timer: bool,
};

let initState = {
  content: "",
  respected: None,
  passed: [],
  failed: None,
  showed: false,
  iteration: 0,
  disabled: false,
  timer: false,
};

let isStateValid = s => s.respected == Some(true) && s.failed == None;

type rule_type =
  | Normal
  | ShowOnFailed;

/**
 * Rule type.
 *
 * Each rule includes a checker and failed reason text.
 */
type rule = {
  c: state => bool, // State checker function
  r: string, // Failed reason
  d: option(state => state), // optional state reducer
  t: rule_type,
};

/** Check for condition defined in predicate over each character of string. */
let strCheck = (s: string, predicate: char => bool) =>
  s |> String.map(ch => predicate(ch) ? 'o' : 'x') |> String.contains(_, 'o');

/** Predefined rules. */
let rules: list(rule) = [
  {
    c: _ => true,
    r: "Please fill out your identity slowly.",
    d: Some(s => {...s, disabled: true, content: ""}),
    t: ShowOnFailed,
  },
  {
    c: s => String.length(s.content) >= 8,
    r: "Password must be at least 8 characters.",
    d: None,
    t: Normal,
  },
  {
    c: s => strCheck(s.content, ch => ch >= 'A' && ch <= 'Z'),
    r: "Password must contain at least one common Latin uppercase letter.",
    d: None,
    t: Normal,
  },
  {
    c: s => strCheck(s.content, ch => ch >= '0' && ch <= '9'),
    r: "Password must contain at least one Latin number.",
    d: None,
    t: Normal,
  },
  {
    c: s => strCheck(s.content, isSpecialChar),
    r: "Password must contain at least one special character found on US keyboard.",
    d: None,
    t: Normal,
  },
  {
    c: s => repetitiveChecker(s.content, None),
    r: "Password must not contain repetitive pattern.",
    d: None,
    t: Normal,
  },
  {
    c: s => String.contains(s.content, 'q'),
    r: "Password must contain character 'q' (lowercase).",
    d: None,
    t: Normal,
  },
  {
    c: s => String.contains(s.content, 'x'),
    r: "Password must contain character 'x' (lowercase).",
    d: None,
    t: Normal,
  },
  {
    c: st =>
      switch (st.respected) {
      | None => false
      | Some(respected) => respected
      },
    r: "Press F to pay respect!",
    d: Some(s => s.respected == None ? {...s, respected: Some(false)} : s),
    t: ShowOnFailed,
  },
  // New Windows update makes it extremely infuriated to enter emoji in a
  // password field. Let's try this!
  {
    c: s => isHasEmoji(s.content),
    r: "Must have one emoji.",
    d: None,
    t: Normal,
  },
  {
    c: s => emoPointer(s.content),
    r: "Must point left or right.",
    d: None,
    t: Normal,
  },
  {
    c: s => !String.contains(s.content, ' '),
    r: "Space is not allowed in password (for an obvious security reason).",
    d: None,
    t: ShowOnFailed,
  },
  {
    c: s => String.length(s.content) <= 14,
    r: "Password must be at most 14 characters.",
    d: None,
    t: Normal,
  },
];

// Utilities for splitting validate map into passes and fails
let rec ruleSplit = (a, allPassed) =>
  switch (a) {
  | [] => (allPassed, None) // All true
  | [(false, rule), ..._] => (allPassed, Some(rule))
  | [(true, rule), ...rest] =>
    switch (rule.t) {
    | ShowOnFailed => ruleSplit(rest, allPassed)
    | Normal => ruleSplit(rest, [rule.r, ...allPassed])
    }
  };

let ruleReasons = List.map(rule => rule.r);

let findFailed = (state: state, content: string) => {
  let d = abs(content->String.length - state.content->String.length);
  switch (d) {
  // 4 is sufficient for a decent emoji...
  | _ when d > 4 => ([], Some(rules->List.hd))
  | _ =>
    List.map(r => r.c({...state, content}), rules)
    ->List.combine(rules)
    ->ruleSplit([])
  };
};

/** Available actions. */
type action =
  | OnChange(string)
  | Respect(int)
  | TimerReset
  | OnPaste(unit => unit)
  | Toggle;

let keydownHandler = (s, d, e) =>
  switch (s.respected) {
  | Some(false) =>
    e->ReactEvent.Keyboard.preventDefault;
    e->ReactEvent.Keyboard.keyCode->Respect->d;
  | _ => ()
  };

let stateCheck = (s, content) => {
  let (passed, failed) = findFailed(s, content);
  let tmp_state = {
    ...s,
    content: s.respected == Some(false) ? s.content : content,
    passed,
  };
  switch (failed) {
  | None => {...tmp_state, failed: None}
  | Some(rule) =>
    let iteration = Some(rule.r) != s.failed ? s.iteration + 1 : s.iteration;
    switch (rule.d) {
    | None => {...tmp_state, iteration, failed: Some(rule.r)}
    | Some(fn) => fn({...tmp_state, iteration, failed: Some(rule.r)})
    };
  };
};

/** Unified reducer. */
let reducer = (s, action) =>
  switch (action) {
  | OnChange(content) => stateCheck(s, content)
  | Respect(keyCode) =>
    stateCheck({...s, respected: Some(keyCode == 70)}, s.content)
  | TimerReset => initState
  | OnPaste(cb) =>
    // This reduce call have side effect!
    let _ = Js.Global.setTimeout(cb, 5000);
    {...s, disabled: true, passed: [], failed: Some(rules->List.hd.r)};
  | Toggle => {...s, showed: !s.showed, timer: true}
  };

let onProgressEnd = (stateReducer, timerSetter, _) => {
  timerSetter(_ => false);
  stateReducer(TimerReset);
};

let enableTimer = (stateReducer, timerSetter, _) => {
  timerSetter(_ => true);
  stateReducer(Toggle);
};

module Timer = {
  /**
   * Animated timer.
   *
   * Simple timer with progress bar, timing by browser's internal
   * CSS animation engine!
   *
   * On timed out, a callback should be triggered (e.g., clear
   * input text, yay!)
   */
  [@react.component]
  let make = (~fs, ~endProgressCb=?) => {
    let onAnimationEnd =
      switch (endProgressCb) {
      | None => (_ => ())
      | Some(fn) => fn
      };

    <div className="progress">
      <div className="progress-bar" disabled={!fs} onAnimationEnd />
    </div>;
  };
};

[@react.component]
let make = (~passSetter, ~showFailed=true) => {
  let (s, d) = React.useReducer(reducer, initState);
  let (timer, timerSetter) = React.useState(() => false);

  <>
    <div className="input-group">
      <label className="prepend preserved-width">
        "Password"->React.string
      </label>
      <input
        type_="password"
        placeholder="Just a simple password..."
        autoComplete="new-password"
        required=true
        minLength=8
        maxLength=25
        value={s.content}
        disabled={s.disabled}
        onChange={e => e->ReactEvent.Form.target##value->OnChange->d}
        onKeyDown={keydownHandler(s, d)}
        onPaste={_ => OnPaste(() => TimerReset->d)->d}
      />
      {s.iteration == 0 && showFailed
         ? React.null
         : <button
             className="append button"
             reversed={s.showed}
             onClick={enableTimer(d, timerSetter)}>
             <i> "Hint"->React.string </i>
           </button>}
    </div>
    {s.timer
       ? <div className="input-group">
           <div className="prepend preserved-width" />
           <Timer endProgressCb={onProgressEnd(d, timerSetter)} fs=timer />
         </div>
       : React.null}
    {s.iteration == 0 && showFailed
       ? ReasonReact.null
       : <div className="input-group">
           <div className="reasons">
             {switch (s.failed) {
              | None =>
                passSetter(_ => true);
                <p> "You are good to go!"->React.string </p>;
              | Some(reason) =>
                <div className="failed" key={s.iteration->string_of_int}>
                  reason->React.string
                </div>
              }}
             {s.passed
              |> fxi((key, pass) =>
                   <div key className="passed"> pass->React.string </div>
                 )}
           </div>
         </div>}
  </>;
};
