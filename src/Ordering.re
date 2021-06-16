type ordering =
  | Greater
  | Less
  | Equal;

// ORD typeclass / interface module
module type ORD = {
  type t;
  let compare: (t, t) => ordering;
};

// Sortable module that requires an ORD typeclass
module Sortable = (Item: ORD) => {

  let orderingToInt = (o: ordering) => {
    switch (o) {
    | Greater => 1
    | Less => (-1)
    | Equal => 0
    };
  };

  let sort = (ts: list(Item.t)) =>
    List.sort((x, y) => Item.compare(x, y) |> orderingToInt, ts);
};

// concrete impementation of ORD typeclass
module Ord_Int = {
  type t = int;
  let compare = (x, y) => x > y ? Greater : x < y ? Less : Equal;
};

// sortable module for ints based
module IntSort = Sortable(Ord_Int);

let sorted = IntSort.sort([3, 2, 1]);
Js.log(sorted);

// module to invert the ordering of an ORD instance
module Invert = (Item: ORD) => {
  type t = Item.t;
  let compare = (x, y) => Item.compare(y, x);
};

// ORD instance module created by inverting an existing ORD instance
module Ord_Int_Desc = Invert(Ord_Int);

// another Sortable module
module IntSort_Desc = Sortable(Ord_Int_Desc);

Js.log(IntSort_Desc.sort([3, 2, 4, 2]));
