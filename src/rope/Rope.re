// @module(Rope) Unicode string manipulator

/** Convert a string into list of 16-bit chars */
let str_to_list = s => {
  let b = String.length(s)->Array.make @@ 0;
  String.iteri((i, ch) => b[i] = Char.code(ch), s);
  Array.to_list(b);
};

let running_encoder = (cur, prev, acc) =>
  switch (cur, prev) {
  | (0, _) => acc // end of pair, or, null char
  | _ when cur <= 0xD7FF => [cur, ...acc] // Lower part of BMP
  | _ when cur <= 0xDFFF => acc // Invalid code point D800..DFFF
  | _ when cur <= 0xFFFF => [cur, ...acc] // Higher part of BMP
  | _ when cur <= 0x10FFFF => [
      cur lsr 10 land 0x3FF lor 0xD800, // high bits
      cur land 0x3FF lor 0xDC00, // low bits
      ...acc,
    ] // encode to two 16-bits char
  | _ => acc // Larger codepoints are ignored
  };

let running_decoder = (cur, prev, acc) => {
  switch (cur, prev) {
  | (0, _) => acc // end of pair, or, null char
  | _ when cur <= 0xD7FF => [cur, ...acc] // lower part of BMP, 0000..D7FF
  | _ when cur <= 0xDBFF => [
      0x10000 lor (prev land 0x3FF) lor (cur land 0x3FF) lsl 10, // cool?
      ...acc,
    ] // Range of high surrogate, D800..DBFF
  | _ when cur <= 0xDFFF => acc // Range of low surrogate DC00..DFFF
  | _ => [cur, ...acc] // higher part of BMP, E000..FFFF
  };
};
/** Decode UTF-16 BE to Unicode codepoint. */
let rev_utf16_be = l =>
  List.fold_right2(running_decoder, [0] @ l, l @ [0], []);

/** Encode Unicode codepoint into UTF-16 BE. */
let fwd_utf16_be = l =>
  List.fold_right2(running_encoder, [0] @ l, l @ [0], []);
