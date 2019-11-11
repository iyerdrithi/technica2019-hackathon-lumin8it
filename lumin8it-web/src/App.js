import React, { useState, useContext } from "react";
import Navbar from "react-bootstrap/Navbar";
import Nav from "react-bootstrap/Nav";
import Form from "react-bootstrap/Form";
import Button from "react-bootstrap/Button";
import FormControl from "react-bootstrap/FormControl";
import { Context as AuthContext } from "./context/AuthContext";
import { Link } from "react-router-dom";
import Image from "react-bootstrap/Image"
import Container from "react-bootstrap/Container"
import Row from "react-bootstrap/Row"
import "./App.css";

function App() {
  const { state, signin } = useContext(AuthContext);
  console.log("MAIN USERNAME:" + state.username);
  const [username, setUsername] = useState(state.username);

  return (
    
    <Navbar bg="light" expand="lg">
      <Navbar.Brand href="/">Lumin8it</Navbar.Brand>
      <Navbar.Toggle aria-controls="basic-navbar-nav" />
      <Navbar.Collapse id="basic-navbar-nav">
        <Nav className="mr-auto">
          <Nav.Link as={Link} to="/">
            Home
          </Nav.Link>
          <Nav.Link as={Link} to="/list">
            Programs
          </Nav.Link>
          <Nav.Link as={Link} to="/create">
            Add Program
          </Nav.Link>
        </Nav>
        <Form inline>
          <FormControl
            type="text"
            placeholder="username"
            value={username}
            onChange={e => {
              e.persist();
              console.log(e.target.value);
              setUsername(e.target.value);
            }}
            className="mr-sm-2"
          />
          <FormControl type="text" placeholder="password" className="mr-sm-2" />
          <Button
            type="button"
            onClick={e => {
              console.log("USERNAME" + username);
              signin(username);
            }}
          >
            login
          </Button>
        </Form>
      </Navbar.Collapse>
    </Navbar>
  
  )
    
}
  

export default App;
