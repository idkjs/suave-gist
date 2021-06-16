type webPart('a) = 'a => Js.Promise.t(option('a));

let pure = a => Js.Promise.resolve(Some(a));
let fail = _ => Js.Promise.resolve(None);
let (>=>) = (x, y, a) =>
  x(a)
  |> Js.Promise.then_(
       fun
       | None => Js.Promise.resolve(None)
       | Some(a) => y(a),
     );

let rec choose = (options, arg) =>
  switch (options) {
  | [] => fail(arg)
  | [h, ...t] =>
    h(arg)
    |> Js.Promise.then_(
         fun
         | Some(a) => pure(a)
         | None => choose(t, arg),
       )
  };

let filter = (pred, arg) => pred(arg) ? pure(arg) : fail(arg);

type http = {
  path: string,
  method: string,
  send: (int, Js.Json.t) => http,
};

let get =
  filter(
    fun
    | {method: "GET"} => true
    | _ => false,
  );

let put =
  filter(
    fun
    | {method: "PUT"} => true
    | _ => false,
  );
let post =
  filter(
    fun
    | {method: "POST"} => true
    | _ => false,
  );

let delete =
  filter(
    fun
    | {method: "DELETE"} => true
    | _ => false,
  );

let path = p => filter(http => http.path == p ? true : false);

let convert = a => {
  switch (Js.Json.stringifyAny(a)) {
  | Some(s) => Js.Json.parseExn(s)
  | None => Js.Json.null
  };
};

let setStatus = (code, content): webPart(http) =>
  http => pure(http.send(code, content |> convert));

let ok = content => setStatus(200, content);
let notFound = content => setStatus(404, content);
let error = content => setStatus(500, content);

// example api

// type store = { number : int }
// let stores = [| { number: 2304 } |]

// let api = choose([
//   path("/stores") >=> choose([
//     get >=> ok(stores),
//     post >=> ok("posted stores")
//   ]),
//   path("/items") >=> choose([
//     get >=> ok("got items"),
//     post >=> ok("posted items")
//   ]),
//   notFound("Like Bono, we still haven't found what you're looking for")
// ])
