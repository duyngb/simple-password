// Simple case first.

module CompUsername = {
  [@react.component]
  let make = () => {
    <div className="input-group">
      <label className="prepend preserved-width">
        "Username"->React.string
      </label>
      <input
        type_="text"
        name="username"
        autoComplete="off"
        required=true
        placeholder="This would be permanent"
      />
    </div>;
  };
};

let cb = e => Js.log(e);

[@react.component]
let make = () => {
  <div className="reg-form">
    <p> "Just two simple fields and you are done!"->React.string </p>
    <div className="rows">
      <CompUsername />
      <PasswordInput />
    </div>
    <div className="rows">
      <div className="input-group">
        <input type_="submit" value="I'm done!" disabled=true />
      </div>
    </div>
    <p className="note"> <i> "All fields are required"->React.string </i> </p>
  </div>;
};
