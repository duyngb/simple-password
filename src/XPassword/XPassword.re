// Simple case first.

module CompUsername = {
  [@react.component]
  let make = () => {
    <div className="input-group">
      <label className="prepend preserved-width"> "Username"->React.string </label>
      <input type_="text" name="username" />
    </div>;
  };
};

[@react.component]
let make = () => {
  <div>
    <div> {React.string("chilled")} </div>
    <div> <CompUsername /> </div>
    <div> <PasswordInput /> </div>
  </div>;
};
