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
    <p> {React.string("chilled")} </p>
    <p> <CompUsername /> </p>
    <p> <PasswordInput /> </p>
  </div>;
};
