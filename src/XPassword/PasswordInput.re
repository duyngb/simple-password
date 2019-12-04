let tail = s => String.sub(s, 1, String.length(s) - 1);

let fxi = (fx, list) =>
  list
  |> List.mapi((idx, a) => fx(string_of_int(idx), a))
  |> Array.of_list
  |> React.array;

let isSpecialChar = ch =>
  switch (ch) {
  | '!'..'/' => true
  | ':'..'@' => true
  | '['..'`' => true
  | '{'..'~' => true
  | _ => false
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

/** Check for condition defined in predicate over each character of string. */
let strCheck = (predicate: char => bool, s: string) =>
  s |> String.map(ch => predicate(ch) ? 'o' : 'x') |> String.contains(_, 'o');

/** rule type */
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
    c: strCheck(ch => ch == Char.uppercase_ascii(ch)),
    r: "Password must contain at least one Latin uppercase letter.",
  },
  {
    c: strCheck(ch => ch >= '0' && ch <= '9'),
    r: "Password must contain at least one Latin number.",
  },
  {
    c: strCheck(isSpecialChar),
    r: "Password must contain at least one special character found on US keyboard.",
  },
  {
    c: s => repetitiveChecker(None, s),
    r: "Password must not contain repetitive pattern.",
  },
  {
    c: s => String.contains(s, 'q'),
    r: "Password must contain character 'q' (lowercase).",
  },
  {
    c: s => String.contains(s, 'x'),
    r: "Password must contain character 'x' (lowercase).",
  },
  {
    c: s => String.length(s) < 14,
    r: "Password must be shorter than 14 characters.",
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
  | [(true, rule), ...rest] => ruleSplit(rest, [rule.r, ...allPassed])
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
      {React.string(s.valid ? "You are good to go" : "Opp")}
    </label>
    <div>
      {switch (s.failed) {
       | None => <div />
       | Some(reason) => <div> {React.string("[x] " ++ reason)} </div>
       }}
      {s.passed |> fxi((key, pass) => <div key> {React.string(pass)} </div>)}
    </div>
  </div>;
};
