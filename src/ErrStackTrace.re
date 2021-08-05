let throwError: unit => unit = [%bs.raw
  {|function () { throw new Error("aaaa"); }|}
];

try (throwError()) {
| Js.Exn.Error(e) =>
  switch (Js.Exn.message(e)) {
  | Some(message) => Js.log({j|Error>>>>>: $message|j})
  | None => Js.log("An unknown error occurred")
  }
};
