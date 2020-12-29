type stage =
  | OnUserName
  | OnPassword
  | FinalStage;

type state_t = {
  key: int,
  uservalid: bool,
  c1valid: bool,
  stage,
};

type action =
  | NextStage
  | UsrContent(string, bool)
  | Content1(string, bool)
  | ResetAll;

let initState = {
  key: 0,
  uservalid: false,
  c1valid: false,
  stage: OnUserName,
};

let reducer = (s, a) => {
  switch (a) {
  | NextStage =>
    let stage =
      switch (s.stage) {
      | OnUserName => OnPassword
      | OnPassword => FinalStage
      | FinalStage => FinalStage
      };
    {...s, stage};
  | UsrContent(_username, uservalid) => {...s, uservalid}
  | Content1(_content1, c1valid) => {...s, c1valid}
  | ResetAll => {...initState, key: s.key + 1}
  };
};

let stateValid = s =>
  switch (s.stage) {
  | OnUserName => s.uservalid
  | OnPassword => s.c1valid
  | FinalStage => true
  };

module CompUsername = {
  let onChange = (onContent, e: ReactEvent.Form.t) => {
    let value: string = e->ReactEvent.Form.target##value;
    let check =
      String.length(value) >= 5
      && !String.contains(value, ' ')
      && PasswordInput.strCheck(value, c => !PasswordInput.isSpecialChar(c));
    onContent(value, check);
  };

  [@react.component]
  let make = (~disabled, ~onContent=(_, _) => ()) => {
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
        onChange={onChange(onContent)}
        disabled
      />
    </div>;
  };
};

/** Should component render? */
let r = (condition, element) => condition ? element : React.null;

[@react.component]
let make = () => {
  let (s, d) = React.useReducer(reducer, initState);

  <div className="reg-form" key={string_of_int(s.key)}>
    <p>
      {switch (s.stage) {
       | OnUserName => "Please choose a name for your eternity"
       | OnPassword => "Please prove yourself"
       | FinalStage => "That is. What a day!"
       }}
      ->React.string
    </p>
    <div className="rows">
      <CompUsername
        disabled={s.stage != OnUserName}
        onContent={(c, v) => UsrContent(c, v)->d}
      />
      {s.stage >= OnPassword
         ? <PasswordInput
             disabled={s.stage != OnPassword}
             onContent={(c, v) => Content1(c, v)->d}
           />
         : React.null}
    </div>
    <div className="rows">
      <div className="input-group">
        <button
          className="button"
          disabled={!s->stateValid}
          onClick={_ => NextStage->d}>
          {switch (s.stage) {
           | OnUserName => "I accept the risk"
           | OnPassword => "I'm done"
           | FinalStage => ""
           }}
          ->React.string
        </button>
        <button className="button append danger" onClick={_ => ResetAll->d}>
          "start over"->React.string
        </button>
      </div>
    </div>
    <p className="note"> <i> "All fields are required"->React.string </i> </p>
  </div>;
};
