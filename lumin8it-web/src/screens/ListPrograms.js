import React, { useState, useEffect } from "react";
import axios from "axios";
import ListGroup from "react-bootstrap/ListGroup";
import Spinner from "react-bootstrap/Spinner";
import { withRouter } from "react-router-dom";
import usePrograms from "../hooks/usePrograms";
import { Context as AuthContext } from "../context/AuthContext";

function ListPrograms(props) {
  const [getPrograms, programResults] = usePrograms();
  console.log(programResults);

  /*
  const [data, setData] = useState([]);
  const [showLoading, setShowLoading] = useState(true);
  const apiUrl = "http://localhost:3000/api/v1/products";

  useEffect(() => {
    const fetchData = async () => {
      const result = await axios(apiUrl);
      setData(result.data);
      setShowLoading(false);
    };

    fetchData();
  }, []);*/

  const showDetail = id => {
    props.history.push({
      pathname: "/show/" + id
    });
  };

  return (
    <div>
      <ListGroup>
        {programResults.map((item, idx) => (
          <ListGroup.Item
            key={idx}
            action
            onClick={() => {
              showDetail(item.programid);
            }}
          >
            {item.programName}
            
            
          </ListGroup.Item>
        ))}
      </ListGroup>
    </div>
  );
}

export default withRouter(ListPrograms);
