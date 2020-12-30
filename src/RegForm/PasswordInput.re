open State;
open Checks;

let fxi = (fx, list) =>
  list
  |> List.mapi((idx, a) => fx(string_of_int(idx), a))
  |> Array.of_list
  |> React.array;

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

let findFailed = (state, content) => {
  let l1 = state.content->String.length;
  let l2 = content->String.length;
  let d = abs(l1 - l2);
  switch () {
  // 4 is sufficient for a decent emoji...
  // TODO: Check over decoded rune, not raw chars
  | _ when l2 > 2 && d > 4 => ([], Some(rules->List.hd))
  | _ =>
    List.map(r => r.c({...state, content}), rules)
    ->List.combine(rules)
    ->ruleSplit([])
  };
};

/** Available actions. */
type action =
  | OnChange(string, (string, bool) => unit)
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
  | OnChange(content, onContent) =>
    let state = stateCheck(s, content);
    onContent(state.content, state.failed == None);
    state;
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
  let make = (~fs, ~onTimerEnd=_ => ()) => {
    <div className="progress">
      <div className="progress-bar" disabled={!fs} onAnimationEnd=onTimerEnd />
    </div>;
  };
};

[@react.component]
let make = (~disabled, ~onContent=(_, _) => ()) => {
  let (s, d) = React.useReducer(reducer, initState);
  let (timer, timerSetter) = React.useState(() => false);

  // These are used to skip first render of "onContent" changing
  // from upper component.
  let (cCb, setCb) = React.useState(((), _, _) => ());
  let firstRender = React.useRef(true);
  React.useEffect1(
    () => {
      if (firstRender.current) {
        firstRender.current = false;
      } else {
        setCb(_ => onContent);
      };
      None;
    },
    [|onContent|],
  );

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
        disabled={disabled || s.disabled}
        onChange={e => OnChange(ReactEvent.Form.target(e)##value, cCb)->d}
        onKeyDown={keydownHandler(s, d)}
        onPaste={_ => OnPaste(() => TimerReset->d)->d}
        onDrop={e => e->ReactEvent.Mouse.preventDefault}
      />
      {s.iteration == 0 || s.disabled
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
           <Timer onTimerEnd={onProgressEnd(d, timerSetter)} fs=timer />
         </div>
       : React.null}
    {s.iteration == 0
       ? ReasonReact.null
       : <div className="input-group">
           <div className="reasons">
             {switch (s.failed) {
              | None => <p> "You are good to go!"->React.string </p>
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
