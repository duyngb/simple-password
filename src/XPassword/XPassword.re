// Simple case first.

module CompUsername = {
  [@react.component]
  let make = () => {
    <label>
      {React.string("Username")}
      <input type_="text" name="username" />
    </label>;
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
