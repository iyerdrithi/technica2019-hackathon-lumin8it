import { useEffect, useState, useContext } from "react";
import lumin8itApi from "../api/lumin8it";
import { Context as AuthContext } from "../context/AuthContext";

export default () => {
  const { state } = useContext(AuthContext);
  const [programResults, setProgramResults] = useState([]);

  const getPrograms = async () => {
    try {
      const response = await lumin8itApi.get("/programs", {
        params: {
          userId: state.username
        }
      });
      console.log("DATRRR");
      console.log(response.data);
      setProgramResults(response.data.body.programs);
    } catch (err) {
      console.log(err);
    }
  };

  useEffect(() => {
    getPrograms();
  }, [getPrograms]);

  return [getPrograms, programResults];
};
