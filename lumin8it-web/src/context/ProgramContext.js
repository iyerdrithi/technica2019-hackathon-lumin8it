import createDataContext from "./createDataContext";
import lumin8itApi from "../api/lumin8it";

const programReducer = (state, action) => {
  switch (action.type) {
    default:
      return state;
  }
};

const createProgram = dispatch => async (
  measurementType,
  lightBehavior,
  lightCount,
  active,
  userId,
  programName,
  callback
) => {
  console.log("POSTITEM");
  console.log(measurementType);
  console.log(lightBehavior);
  console.log(lightCount);
  console.log(active);
  console.log(userId);

  await lumin8itApi.post("/programs", {
    measurementType,
    lightBehavior,
    lightCount,
    active,
    userId,
    programName
  });
  if (callback) {
    callback();
  }
};

export const { Provider, Context } = createDataContext(
  programReducer,
  {
    createProgram
  },
  []
);
