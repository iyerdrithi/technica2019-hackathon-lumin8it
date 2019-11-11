const uuidv4 = require("uuid/v4");
const AWS = require("aws-sdk");
AWS.config.update({ region: "us-east-1" });

const dynamo = new AWS.DynamoDB.DocumentClient();

exports.handler = (event, context, callback) => {
  const done = (err, res) => {
    console.log(err);

    callback(null, {
      statusCode: err ? "400" : "200",
      body: err ? err.message : JSON.stringify(res),
      headers: {
        "Content-Type": "application/json"
      }
    });
  };

  const program = { ...JSON.parse(event.body) }; //JSON.parse(event.body) for API Gateway, just event.body for lambda
  console.log(program);

  if (event.httpMethod === "POST") {
    const programid = uuidv4();
    program.programid = programid;
  }

  program.timestamp = new Date().getTime();

  console.log(program);

  if (event.httpMethod === "POST") {
    console.log("I TRIED A POST");
    dynamo.put({ TableName: "lumin8itprograms", Item: program }, done);
  } else if (event.httpMethod === "PUT") {
    var updateParams = {
      TableName: "lumin8itprograms",
      Key: { programid: program.programid },
      UpdateExpression:
        "set #measurementType = :measurementType, #lightBehavior = :lightBehavior, #lightCount = :lightCount, #active = :active, #timestamp = :timestamp, #userId = :userId",
      ExpressionAttributeNames: {
        "#measurementType": "measurementType",
        "#lightBehavior": "lightBehavior",
        "#lightCount": "lightCount",
        "#active": "active",
        "#timestamp": "timestamp",
        "#userId": "userId"
      },
      ExpressionAttributeValues: {
        ":measurementType": program.measurementType,
        ":lightBehavior": program.lightBehavior,
        ":lightCount": program.lightCount,
        ":active": program.active,
        ":timestamp": program.timestamp,
        ":userId": program.userId
      }
    };
    dynamo.update(updateParams, done);
  }
};
