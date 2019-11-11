import React, { useState, useEffect } from "react";
import axios from "axios";
import { withRouter } from "react-router-dom";
import Container from "react-bootstrap/Container";
import Nav from 'react-bootstrap/Nav';
import Row from 'react-bootstrap/Row';
import { Link } from "react-router-dom";



function Instructions() {

    return(
    <Container> 
        <Row>
            <img width="40%" src={ require('../images/logo-rectangle.png') } />
        </Row>
        <h4> Sound-Based Sensors  </h4>
        <p> Our sound based sensors change the light effects and colors on your shoes in reaction to the sound in the environment.</p>
        <p> 3D Image Link to Sound-Based Sensors: <Nav.Link>https://fyu.se/v/lgnd1xbws1</Nav.Link></p>
        
        <h4> Pressure-Based Sensors </h4>
        <p> Our pressure based sensors change the lights on your shoes based on pressure applied to the sole of the shoe when you walk or run. </p>
        <p> 3D Image Link to Pressure-Based Sensors: <Nav.Link> https://fyu.se/v/aahlgbk278 </Nav.Link> </p>
        
        <h4> Light Patterns </h4>
        <p> Lights patterns on your shoes have been programmed based on number of lights and a color picker. You can choose up to three colors on the color picker and your shoes will change patterns based on the program.</p>
        <p> 3D Image Link to Light Patterns: <Nav.Link> https://fyu.se/v/ivvfsp357d </Nav.Link> </p>
    </Container>
    );

}

export default withRouter(Instructions);
