const doc = require("dynamodb-doc");
const http = require("http");

const dynamo = new doc.DynamoDB();

exports.handler = (event, context, callback) => {
  const done = (err, res) => {
    console.log(res);

    const objectResponse = {
      programs: res.Items,
      timestamp: Math.max.apply(
        Math,
        res.Items.map(function(o) {
          return o.timestamp;
        })
      ),
      programCount: res.Items.length
    };

    callback(null, {
      statusCode: err ? "400" : "200",
      body: err ? err.message : objectResponse,
      headers: {
        "Content-Type": "application/json"
      }
    });
  };

  console.log(event.queryStringParameters);

  if (event.queryStringParameters) {
    if (event.queryStringParameters.userId) {
      console.log(event.queryStringParameters.userId);
      const params = {
        ExpressionAttributeValues: {
          ":userId": event.queryStringParameters.userId
        },
        FilterExpression: "userId = :userId",
        TableName: "lumin8itprograms",
        IndexName: "userId-index"
      };
      dynamo.scan(params, done);
    }
  } else {
    dynamo.scan({ TableName: "lumin8itprograms", Limit: "20" }, done);
  }
};
