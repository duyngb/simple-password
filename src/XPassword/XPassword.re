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
  <div className="reg-form">
    <p>
      "Just a simple username and password and you are done!"->React.string
    </p>
    <div className="rows">
      <CompUsername />
      <PasswordInput />
    </div>
  </div>;
};
