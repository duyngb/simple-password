let tail = s => String.sub(s, 1, String.length(s) - 1);

let fxi = (fx, list) =>
  list
  |> List.mapi((idx, a) => fx(string_of_int(idx), a))
  |> Array.of_list
  |> React.array;

let diff = (_n, _o) => [];

let rec yankPasted = (newContent, oldContent) => {
  let n = newContent |> Rope.str_to_list |> Rope.rev_utf16_be;
  let o = oldContent |> Rope.str_to_list |> Rope.rev_utf16_be;

  let l = List.compare_lengths(n, o);
  Js.log(l);

  switch (l) {
  | (-1) => yankPasted(oldContent, newContent)
  | 0
  | 1 => false
  | 2 => Emo.emoji_folder(diff(n, o)) == []
  | _ => true
  };
};

let isSpecialChar = ch =>
  switch (ch) {
  | '!'..'/' => true
  | ':'..'@' => true
  | '['..'`' => true
  | '{'..'~' => true
  | _ => false
  };

let rec repetitiveChecker = (lastChar, s) =>
  switch (lastChar, s) {
  | (_, "") => true
  | (None, s) => repetitiveChecker(Some(s.[0]), tail(s))
  | (Some(c), s) =>
    let hd = s.[0];
    let tl = tail(s);
    c == hd ? false : repetitiveChecker(Some(hd), tl);
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
  respected: option(bool), // Did someone respected? If None, do not check for keydown
  passed: list(string), // passed rules
  failed: option(string), // current failed rule
  showed: bool, // is this field currently plain text
  iteration: int,
  disabled: bool,
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

/**
 * Rule type.
 *
 * Each rule includes a checker and failed reason text.
 */
type rule = {
  c: state => bool, // State checker function
  r: string // Failed reason
};

let ($) = (f, g, x) => f(x) |> g;
let stc = s => s.content;

/** Check for condition defined in predicate over each character of string. */
let strCheck = (predicate: char => bool, s: string) =>
  s |> String.map(ch => predicate(ch) ? 'o' : 'x') |> String.contains(_, 'o');

/** Predefined rules. */
let rules: list(rule) = [
  {
    c: s => String.length(s.content) >= 8,
    r: "Password must be at least 8 characters.",
  },
  {
    c: stc $ strCheck(ch => ch >= 'A' && ch <= 'Z'),
    r: "Password must contain at least one common Latin uppercase letter.",
  },
  {
    c: stc $ strCheck(ch => ch >= '0' && ch <= '9'),
    r: "Password must contain at least one Latin number.",
  },
  {
    c: stc $ strCheck(isSpecialChar),
    r: "Password must contain at least one special character found on US keyboard.",
  },
  {
    c: stc $ repetitiveChecker(None),
    r: "Password must not contain repetitive pattern.",
  },
  {
    c: stc $ String.contains(_, 'q'),
    r: "Password must contain character 'q' (lowercase).",
  },
  {
    c: stc $ String.contains(_, 'x'),
    r: "Password must contain character 'x' (lowercase).",
  },
  {
    c: st =>
      switch (st.respected) {
      | None => false
      | Some(respected) => respected
      },
    r: "Press F to pay respect!",
  },
  {c: stc $ isHasEmoji, r: "Must have one emoji."},
  {c: stc $ emoPointer, r: "Must point left or right, but not up or down."},
  {
    c: stc $ strCheck(ch => ch == ' ') $ (!),
    r: "Space is not allowed in password (for an obvious security reason).",
  },
  {
    c: s => String.length(s.content) <= 14,
    r: "Password must be at most 14 characters.",
  },
];

// Utilities for splitting validate map into passes and fails
let rec ruleSplit = (a, allPassed) =>
  switch (a) {
  | [] => (allPassed, None) // All true
  | [(false, rule), ..._] => (allPassed, Some(rule.r))
  | [(true, rule), ...rest] => ruleSplit(rest, [rule.r, ...allPassed])
  };

/** Available actions. */
type action =
  | OnChange(string)
  | Respect(int)
  | TimerReset
  | OnPaste(action => unit)
  | Toggle;

let ruleCheck = state => rules |> (r => r.c(state))->List.map;

/** Unified reducer. */
let reducer = (s, action) =>
  switch (action) {
  | OnChange(content) =>
    if (yankPasted(content, s.content)) {
      {...s, content: ""};
    } else {
      let (passed, failed) =
        {...s, content}->ruleCheck->List.combine(rules)->ruleSplit([]);
      let iteration = failed != s.failed ? s.iteration + 1 : s.iteration;
      // Enable key hook when it's required to pay respect, otherwise, keep it.
      let (respected, content) =
        switch (failed, s.respected) {
        | (Some("Press F to pay respect!"), None) => (Some(false), content)
        | (Some("Press F to pay respect!"), Some(r)) => (
            Some(r),
            String.sub(content, 0, String.length(content) - 1),
          )
        | (_, r) => (r, content)
        };
      {...s, content, passed, failed, iteration, respected};
    }
  | Respect(keyCode) => {...s, respected: Some(keyCode == 70)}
  | TimerReset => initState
  | OnPaste(dispatcher) =>
    // This reduce call have side effect!
    let _ = Js.Global.setTimeout(() => dispatcher(TimerReset), 5000);
    {...initState, content: s.content, disabled: true};
  | Toggle => {...s, showed: !s.showed, timer: true}
  };

[@react.component]
let make = () => {
  let (s, d) = React.useReducer(reducer, initState);
  let (timerEnabled, timerSetter) = React.useState(() => false);

  <div className="input-group-rows">
    <div className="input-group">
      <label className="prepend preserved-width">
        "Password"->React.string
      </label>
      <div className="input-group-row">
        <input
          type_={s.showed ? "text" : "password"}
          name="password"
          placeholder="Just a simple password..."
          required=true
          minLength=8
          maxLength=25
          value={s.content}
          disabled={s.disabled}
          onChange={e => e->ReactEvent.Form.target##value->OnChange->d}
          onKeyDown={
            switch (s.respected) {
            | Some(false) => (e => e->ReactEvent.Keyboard.keyCode->Respect->d)
            | _ => (_ => ())
            }
          }
          onPaste={_ => d->OnPaste->d}
        />
        {s.timer
           ? <Timer
               endProgressCb={_ => {
                 Js.log("animation ended");
                 timerSetter(_ => false);
                 d(TimerReset);
               }}
               fs=timerEnabled
               fd=timerSetter
             />
           : ReasonReact.null}
      </div>
      <button
        className="append button"
        reversed={s.showed}
        onClick={_ => {
          timerSetter(_ => true);
          d(Toggle);
        }}>
        <i> "Hint"->React.string </i>
      </button>
    </div>
    (s->isStateValid ? "You are good to go" : "Opp")->React.string
    <div className="reasons">
      {switch (s.failed) {
       | None => ReasonReact.null
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
  </div>;
};
