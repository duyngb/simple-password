[@bs.val]
external requestAnimationFrame: (int => unit) => unit =
  "window.requestAnimationFrame";
let nRequestAnimationFrame = f =>
  requestAnimationFrame(_ => requestAnimationFrame(t => f(t)));

let (=?=) = (f, default) =>
  switch (f) {
  | None => default
  | Some(f_) => f_
  };

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
let make = (~endProgressCb=?, ~hasToggleBtn=?, ~fs=?, ~fd=?) => {
  let (enabled, setEState) = switch (fs, fd) {
  | (Some(s), Some(d)) => (s, d)
  | _ => React.useState(() => false)
  };
  let onAnimationEnd = endProgressCb =?= (_ => ());
  let btn_ = hasToggleBtn =?= false;

  <div className="progress">
    <div className="progress-bar" disabled={!enabled} onAnimationEnd />
    {btn_
       ? <button className="append" onClick={_ => setEState(e => !e)}>
           "Toggle progress"->React.string
         </button>
       : ReasonReact.null}
  </div>;
};
