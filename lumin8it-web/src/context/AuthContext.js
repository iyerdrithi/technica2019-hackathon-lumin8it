import createDataContext from "./createDataContext";

const authReducer = (state, action) => {
  switch (action.type) {
    case "signin":
      return {
        username: action.payload.username
      };
    case "signout":
      return {
        username: ""
      };
    default:
      return state;
  }
};

const signout = dispatch => () => {
  dispatch({ type: "signout" });
};

const signin = dispatch => username => {
  dispatch({
    type: "signin",
    payload: {
      username: username
    }
  });
};

export const { Provider, Context } = createDataContext(
  authReducer,
  { signin, signout },
  { username: "" }
);
