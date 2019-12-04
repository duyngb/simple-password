let id = i => i;
let head = s => String.sub(s, 0, 1);
let tail = s => String.sub(s, 1, String.length(s) - 1);

let rec hasCap = (prev, s) =>
  switch (prev, s) {
  | (a, "") => a
  | (true, _) => true
  | (false, s) =>
    let hd = head(s);
    let tl = tail(s);
    hasCap(hd == String.capitalize(hd), tl);
  };

let rec hasSpecialChar = (prev, s) =>
  switch (prev, s) {
  | (a, "") => a
  | (true, _) => true
  | (_, s) =>
    let hd = s.[0];
    let tl = tail(s);
    let hasThatChar =
      hd >= '!'
      && hd <= '/'
      || hd >= ':'
      && hd <= '@'
      || hd >= '['
      && hd <= '`'
      || hd >= '{'
      && hd <= '~';
    hasSpecialChar(hasThatChar, tl);
  };

let rec repetitiveChecker = (lastChar, s) =>
  switch (lastChar, s) {
  | (_, "") => true
  | (None, s) => repetitiveChecker(Some(s.[0]), tail(s))
  | (Some(c), s) =>
    let hd = s.[0];
    let tl = tail(s);
    c == hd ? false : repetitiveChecker(Some(hd), tl);
  };

type rule = {
  c: string => bool, // Checker
  r: string // Failed reason
};

let rules: list(rule) = [
  {
    c: s => String.length(s) > 8,
    r: "Password must be longer than 8 characters.",
  },
  {
    c: s => String.length(s) < 14,
    r: "Password must be shorter than 14 characters.",
  },
  {
    c: s => hasCap(false, s),
    r: "Password must contain at least one Latin uppercase letter.",
  },
  {
    c: s => hasSpecialChar(false, s),
    r: "Password must contain at least one special character found on US keyboard.",
  },
  {
    c: s => repetitiveChecker(None, s),
    r: "Password must not contain repetitive pattern.",
  },
  {
    c: s => String.contains(s, 'Q'),
    r: "Password must contain character 'Q'.",
  },
  {
    c: s => String.contains(s, 'x'),
    r: "Password must contain character 'x'.",
  },
];

// Unified password field state.
type state = {
  content: string, // password content
  valid: bool, // is this field valid?
  passed: list(string), // passed rules
  failed: option(string), // current failed rule
  showed: bool // is this field currently plain text
};

let initState = {
  content: "",
  valid: false,
  passed: [],
  failed: None,
  showed: false,
};

// Utilities for spliting validateMap into passes and fails
let rec ruleSplit = (a, allPassed) =>
  switch (a) {
  | [] => (allPassed, None) // All true
  | [(false, rule), ..._] => (allPassed, Some(rule.r))
  | [(true, rule), ...rest] => ruleSplit(rest, allPassed @ [rule.r])
  };

// Available action
type action =
  | OnUserInput(string)
  | Toggle;

let reducer = (s, action) =>
  switch (action) {
  | OnUserInput(c) =>
    let validateMap = List.map(rule => rule.c(c), rules);
    let (p, f) = ruleSplit(List.combine(validateMap, rules), []);
    {
      ...s,
      content: c,
      valid: List.for_all(i => i, validateMap),
      passed: p,
      failed: f,
    };

  | Toggle => {...s, showed: !s.showed}
  };

[@react.component]
let make = () => {
  let (s, d) = React.useReducer(reducer, initState);

  <div>
    <label>
      {React.string("Password")}
      <input
        type_={s.showed ? "string" : "password"}
        name="password"
        required=true
        onChange={e => d(OnUserInput(e->ReactEvent.Form.target##value))}
      />
      <input
        type_="checkbox"
        name="show-text"
        checked={s.showed}
        onChange={_ => d(Toggle)}
      />
      {React.string(s.valid ? "Go on! No time for waiting" : "Opp")}
    </label>
    <div>
      {List.mapi(
         (idx, pass) =>
           <div key={string_of_int(idx)}>
             {React.string("[ok] " ++ pass)}
           </div>,
         s.passed,
       )
       ->Array.of_list
       ->React.array}
    </div>
    <div>
      {switch (s.failed) {
       | None => <div />
       | Some(reason) => <div> {React.string("[x] " ++ reason)} </div>
       }}
    </div>
  </div>;
};
