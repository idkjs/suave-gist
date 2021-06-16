// https://dev.to/paul_melero/how-to-npm-install-from-github-repositories-or-gists-directly-6og
// example api
open Suave;
type store = { number : int }
let stores = [| { number: 2304 } |]

let api = choose([
  path("/stores") >=> choose([
    get >=> ok(stores),
    post >=> ok("posted stores")
  ]),
  path("/items") >=> choose([
    get >=> ok("got items"),
    post >=> ok("posted items")
  ]),
  notFound("Like Bono, we still haven't found what you're looking for")
])
