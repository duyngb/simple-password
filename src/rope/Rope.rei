/** Convert a string into list of 16-bit chars. These chars  are
 * internal representation of JavaScript string, encoded in UTF-16.BE
 */
let str_to_list: string => list(int);

/** Decode UTF-16 BE to Unicode codepoint.
 *
 * As designed, this encoding scheme support up to code point U+10FFFF.
 *
 * Ref: [UTF-16](https://en.wikipedia.org/wiki/UTF-16)
 */
let rev_utf16_be: list(int) => list(int);

/** Encode Unicode codepoint into UTF-16 BE.
 *
 * As designed, this encoding scheme support up to code point U+10FFFF.
 *
 * Ref: [UTF-16](https://en.wikipedia.org/wiki/UTF-16)
 */
let fwd_utf16_be: list(int) => list(int);
