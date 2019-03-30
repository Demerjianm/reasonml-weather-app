type weather = {
    summary: string,
    temp: float
}


module Decode = {
    let idk = json: weather => 
        Json.Decode.{
            summary: json |> field("description", string),
            temp:    json |> field("id", float) 
    }
}

module Encode = {
    let obj = (c: weather) => {
      Json.Encode.(
        object_([
          ("summary", string(c.summary)),
          ("temp", float(c.temp))
        ])
      );
    };
  }


let url = "https://api.openweathermap.org/data/2.5/weather?q=London,uk&APPID=46a4e5d1da7fab33ce45a5c1b455583b"

let parseWeatherJson = json: weather => {
    Json.Decode.{
        summary: field("text", string, json),
        temp: float_of_string(field("temp", string, json))
    }
}

let parseWeatherResultsJson = json => {
    Json.parseOrRaise(json) |> Json.Decode.(at([
        "query",
        "results",
        "channel",
        "item",
        "condition"
    ], parseWeatherJson))
}

let getWeather = () => 
    Js.Promise.(
        Axios.get(url)
        |> then_(response => {
            let hi = response##data##weather[0] |> Decode.idk
            let res = hi |> Encode.obj
                  resolve(hi)
              }
          )

    )


let test = () => 
    Js.Promise.(
    Axios.get(url)
        |> then_(response => {
            Js.log(response##data)
            let hi = response##data##weather[0] |> Decode.idk
            let res = hi |> Encode.obj
            resolve(Js.log(res))
            /* let yo = checkit(response##data) */
            /* resolve(Js.log(response##data##weather[0])) */
        })
        |> catch((error) => resolve(Js.log(error)))
);
    