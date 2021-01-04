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

let isStateValid = s =>
  s.respected == Some(true)
  && s.failed == None
  && s.iteration > initState.iteration;

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
  | OnChange(string)
  | Respect(int)
  | TimerReset
  | OnPaste(unit => unit)
  | OnDisabledSet(bool)
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
    passed: passed->List.rev,
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
  | Respect(k) => stateCheck({...s, respected: Some(k == 70)}, s.content)
  | TimerReset =>
    let state = stateCheck(s, s.content);
    {
      ...initState,
      iteration: state.iteration,
      passed: [],
      failed: state.failed,
    };
  | OnPaste(cb) =>
    // This reduce call have side effect!
    let _ = Js.Global.setTimeout(cb, 5000);
    {
      ...s,
      disabled: true,
      passed: [],
      failed: Some(rules->List.hd.r),
      // increase iteration to display error on initial paste
      iteration: s.iteration + 1,
    };
  | OnDisabledSet(disabled) => {...s, disabled}
  | Toggle => {...s, showed: !s.showed, timer: true}
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
  let make = (~onTimerEnd=_ => ()) => {
    <div className="progress">
      <div className="progress-bar" onAnimationEnd=onTimerEnd />
    </div>;
  };
};

[@react.component]
let make =
    (
      ~disabled=false,
      ~passed=false,
      ~name="password",
      ~repeat=false,
      ~onContent=(_, _) => (),
    ) => {
  let (s, d) =
    React.useReducer(
      reducer,
      {...initState, respected: repeat ? Some(true) : None},
    );

  // disabled props changes is a signal on stage changed;
  // timer should be cancled on stage progress, too
  React.useEffect1(
    () => {
      OnDisabledSet(disabled)->d;
      None;
    },
    [|disabled|],
  );

  React.useEffect1(
    () => {
      onContent(s.content, isStateValid(s));
      None;
    },
    [|s|],
  );

  <>
    <div className="input-group">
      <label className="prepend preserved-width">
        "Password"->React.string
      </label>
      <input
        name
        type_={s.showed ? "text" : "password"}
        placeholder={
          repeat
            ? "Repeat your above secret"  //
            : "Just a simple password..." //
        }
        autoComplete="new-password"
        required=true
        minLength=8
        maxLength=25
        value={s.content}
        disabled={s.disabled}
        onChange={e => OnChange(ReactEvent.Form.target(e)##value)->d}
        onKeyDown={keydownHandler(s, d)}
        onPaste={_ => OnPaste(() => TimerReset->d)->d}
        onDrop={e => e->ReactEvent.Mouse.preventDefault}
      />
      {s.iteration <= initState.iteration || s.disabled || repeat
         ? React.null
         : <button
             className="append button"
             reversed={s.showed}
             onClick={_ => Toggle->d}>
             <i> "Hint"->React.string </i>
           </button>}
    </div>
    {s.timer
       ? <div className="input-group">
           <div className="prepend preserved-width" />
           <Timer onTimerEnd={_ => TimerReset->d} />
         </div>
       : React.null}
    {s.iteration == initState.iteration || repeat || passed
       ? ReasonReact.null
       : <div className="input-group">
           <div className="reasons">
             {s.passed
              |> fxi((key, pass) =>
                   <div key className="passed"> pass->React.string </div>
                 )}
             {switch (s.failed) {
              | None => <p> "You are good to go!"->React.string </p>
              | Some(reason) =>
                <div className="failed" key={s.iteration->string_of_int}>
                  reason->React.string
                </div>
              }}
           </div>
         </div>}
    {s.iteration >= initState.iteration && repeat
       ? switch (s.failed) {
         | None => React.null
         | Some(reason) =>
           <div className="input-group">
             <div className="reasons">
               <div className="failed" key={s.iteration->string_of_int}>
                 reason->React.string
               </div>
             </div>
           </div>
         }
       : React.null}
  </>;
};
