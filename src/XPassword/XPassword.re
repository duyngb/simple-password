// Simple case first.

module CompUsername = {
  [@react.component]
  let make = () => {
    <div className="input-group">
      <label className="prepend preserved-width">
        "Username"->React.string
      </label>
      <input type_="text" name="username" autoComplete="off" />
    </div>;
  };
};

let cb = e => Js.log(e);

[@react.component]
let make = () => {
  let style =
    ReactDOMRe.Style.make(
      ~maxWidth="420px",
      ~margin="auto",
      ~marginLeft="1em",
      (),
    );

  <div style>
    <p>
      "Just a simple username and password and you are done!"->React.string
    </p>
    <CompUsername />
    <PasswordInput />
  </div>;
};
