type stage =
  | OnUserName
  | OnPassword
  | OnPassword2
  | FinalStage;

type state_t = {
  key: int,
  uservalid: bool,
  c1: string,
  c1valid: bool,
  c2: string,
  c2valid: bool,
  stage,
};

type action =
  | NextStage
  | UsrContent(string, bool)
  | Content1(string, bool)
  | Content2(string, bool)
  | ResetAll;

let initState = {
  key: 0,
  uservalid: false,
  c1: "",
  c1valid: false,
  c2: "",
  c2valid: false,
  stage: OnUserName,
};

let reducer = (s, a) => {
  switch (a) {
  | NextStage =>
    let stage =
      switch (s.stage) {
      | OnUserName => OnPassword
      | OnPassword => OnPassword2
      | OnPassword2 => FinalStage
      | FinalStage => FinalStage
      };
    if (stage == s.stage) {
      {...initState, key: s.key + 1};
    } else {
      {...s, stage};
    };
  | UsrContent(_username, uservalid) => {...s, uservalid}
  | Content1(c1, c1valid) => {...s, c1, c1valid}
  | Content2(c2, c2valid) => {...s, c2, c2valid}
  | ResetAll => {...initState, key: s.key + 1}
  };
};

let stateValid = s =>
  s.stage == FinalStage
    ? true
    : s.uservalid
      && (s.stage >= OnPassword ? s.c1valid : true)
      && (s.stage >= OnPassword2 ? s.c2valid : true);

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
       | OnPassword2 => "Please repeat yourself for confirmation"
       | FinalStage => "That is. What a day!"
       }}
      ->React.string
    </p>
    <div className="rows">
      <UsernameInput
        disabled={s.stage != OnUserName}
        onContent={(c, v) => UsrContent(c, v)->d}
      />
      {s.stage >= OnPassword
         ? <PasswordInput
             disabled={s.stage != OnPassword}
             passed={s.stage > OnPassword}
             onContent={(c, v) => Content1(c, v)->d}
           />
         : React.null}
      {s.stage >= OnPassword2
         ? <PasswordInput
             name="password2"
             repeat=true
             disabled={s.stage != OnPassword2}
             passed={s.stage > OnPassword2}
             onContent={(c, v) => Content2(c, v)->d}
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
           | OnPassword2 => "I'm done"
           | FinalStage => "What a day!"
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
