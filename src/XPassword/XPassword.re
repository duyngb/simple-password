// Simple case first.

type x_state =
  | OnUserName
  | OnPassword
  | FinalStage;

module CompUsername = {
  let onChange = (d, e: ReactEvent.Form.t) => {
    let value: string = e->ReactEvent.Form.target##value;
    let check =
      String.length(value) >= 5
      && !String.contains(value, ' ')
      && PasswordInput.strCheck(value, c => !PasswordInput.isSpecialChar(c));
    d(_ => check);
  };

  [@react.component]
  let make = (~disabled, ~userSetter) => {
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
        onChange={onChange(userSetter)}
        disabled
      />
    </div>;
  };
};

let freezeUsrField = (usrEnabler, passEnabler, _) => {
  usrEnabler(_ => false);
  passEnabler(_ => true);
};

[@react.component]
let make = () => {
  let (s, d) = React.useState(() => OnUserName);
  let (userValid, userSetter) = React.useState(() => false);
  let (passValid, passSetter) = React.useState(() => false);

  <div className="reg-form">
    <p>
      {switch (s) {
       | OnUserName => "Please choose a name for your eternity"
       | OnPassword => "Please prove yourself"
       | FinalStage => "That is. What a day!"
       }}
      ->React.string
    </p>
    <div className="rows">
      <CompUsername disabled={s != OnUserName} userSetter />
      {s >= OnPassword ? <PasswordInput passSetter /> : React.null}
    </div>
    <div className="rows">
      {switch (s) {
       | OnUserName =>
         <div className="input-group">
           <button
             className="button"
             disabled={!userValid}
             onClick={_ => d(_ => OnPassword)}>
             "I accept the risk"->React.string
           </button>
         </div>
       | OnPassword =>
         <div className="input-group">
           <button
             className="button"
             type_="submit"
             disabled={!passValid}
             onClick={_ => d(_ => FinalStage)}>
             "I'm done"->React.string
           </button>
         </div>
       | FinalStage => React.null
       }}
    </div>
    <p className="note"> <i> "All fields are required"->React.string </i> </p>
  </div>;
};
