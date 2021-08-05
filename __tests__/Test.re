open Jest;

let getToken = (): Js.Promise.t(string) =>
  Js.Promise.resolve("some initial data");
// let getDataJs = () => Promise.Js.fromBsPromise(getToken());
let getData = (): Promise.promise(string) =>
  Promise.resolved("some initial data");

type firstStepResult = {first: string};

type secondStepResult = {second: string};

let doFirstStep = (v: string) => {
  Js.log(v);
  Promise.resolved(Ok({first: "first"}));
};

let doSecondStep = (v: firstStepResult) => {
  Js.log(v);
  Promise.resolved(Ok({second: "second"}));
};

let errorOnFirstStep = (v: string) => {
  Promise.resolved(Error(v));
};

describe("Async workflow", () => {
  testPromise("Ok", () => {
    Expect.(
      getData()
      ->Promise.flatMap(doFirstStep)
      ->Promise.flatMapOk(doSecondStep)
      ->Promise.flatMap(x => {
          Promise.resolved(
            switch (x) {
            | Ok(x) => expect(x) |> toEqual({second: "second"})
            | _ => fail("error")
            },
          )
        })
      ->Promise.Js.toBsPromise
    )
  });

  testPromise("Errors", () => {
    Expect.(
      getData()
      ->Promise.flatMap(errorOnFirstStep)
      ->Promise.flatMapOk(doSecondStep)
      ->Promise.flatMap(x => {
          Promise.resolved(
            switch (x) {
            | Error(x) => expect(x) |> toBe("some initial data")
            | _ => fail("Was not Error")
            },
          )
        })
      ->Promise.Js.toBsPromise
    )
  });
  testPromise("Errors Js", () => {
    Expect.(
      getToken()
      ->Promise.Js.fromBsPromise
      ->Promise.Js.toResult

      ->Promise.map(x =>
          switch (x) {
          | Ok(x) => expect(Obj.magic(x)) |> toBe("some initial data")
          | Error(x) => expect(Obj.magic(x)) |> toBe("some initial data")
          // | _ => fail("Was not Error")
          }
        )
      ->Promise.Js.toBsPromise
    )
  });
  // testPromise("Errors Js", () => {
  //   Expect.(
  //     getToken()
  //     ->Promise.Js.fromBsPromise
  //     ->Promise.Js.toResult
  //     // ->Promise.flatMap(errorOnFirstStep)
  //     // ->Promise.flatMapOk(doSecondStep)
  //     ->Promise.flatMap(x => {
  //         Promise.resolved(
  //           switch (x) {
  //           | Error(x) => expect(x) |> toBe("some initial data")
  //           | _ => fail("Was not Error")
  //           },
  //         )
  //       })
  //     ->Promise.Js.toBsPromise
  //   )
  // });
});
