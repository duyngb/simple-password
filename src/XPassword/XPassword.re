// Simple case first.

module CompUsername = {
  [@react.component]
  let make = () => {
    <div className="input-group">
      <label className="prepend preserved-width">
        "Username"->React.string
      </label>
      <input type_="text" name="username" />
    </div>;
  };
};

[@bs.val]
external requestAnimationFrame: (int => unit) => unit =
  "window.requestAnimationFrame";

let cb = e => Js.log(e);
let resetter = _ =>
  (
    _ =>
      (
        _ => {
          Js.log("resetting...");
        }
      )
      ->requestAnimationFrame
  )
  ->requestAnimationFrame;

[@react.component]
let make = () => {
  let (s, d) = React.useState(() => "");
  let progressBar =
    <div
      className={"progress-bar " ++ s}
      onAnimationIteration={e => e->ReactEvent.Animation.elapsedTime->cb}
    />;

  <div>
    <div> {React.string("chilled")} </div>
    <div> <CompUsername /> </div>
    <div> <PasswordInput /> </div>
    progressBar
    <button onClick={_ => {d(s => s == "" ? "running" : "")}}>
      "Reset progress"->React.string
    </button>
  </div>;
};
