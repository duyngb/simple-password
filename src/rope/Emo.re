type state_type =
  | Relax
  | EatNext;

type pstate = {
  acc: list(list(int)), // Overall accumulator
  buff: list(int), // buffer
  state: state_type,
};

/** Core switch for emoji_folder */
let emoji_folder_switch = (cur: int, acc: pstate) =>
  switch (acc.state) {
  | Relax =>
    switch (cur) {
    | 0xFE0E
    | 0xFE0F => {...acc, buff: [cur], state: EatNext}
    | 0x1F3FB
    | 0x1F3FC
    | 0x1F3FD
    | 0x1F3FE
    | 0x1F3FF => {...acc, buff: [cur], state: EatNext}
    | 0x200D =>
      // ZERO WIDTH JOINER found, eat previous and clean up if necessary.
      let (buff, rest) =
        switch (acc.acc) {
        | [] => ([], [])
        | [a, ...b] => (a, b)
        };
      {acc: rest, buff: [cur, ...buff], state: EatNext};
    | cur when cur >= 0x1F230 && cur < 0x1FAA0 => {
        ...acc,
        acc: [[cur], ...acc.acc],
      }
    | _ => acc
    }
  | EatNext =>
    let pair = [cur, ...acc.buff];
    {acc: [pair, ...acc.acc], buff: [], state: Relax};
  };

let emoji_folder = l =>
  List.fold_right(emoji_folder_switch, l, {acc: [], buff: [], state: Relax})
  |> (acc => acc.acc);
