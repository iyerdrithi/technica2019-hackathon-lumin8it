# lumin8it

Light up your world with lumin8it. Hardware doesn't need to be hard. We wanted to create a way for kids to click to code their hardware to help promote a positive first time hardware experience. We chose to create a kit to make light up shoes because they're a daily wear item that sparks joy when they glow. A light up shoe build also translates well to adults because of the reminder of childhood joy. We added a button to run through multiple programs and a variety of sensors so the shoes could react to the world around them. Our instructions are 3D so kids can see all angles of assembling the hardware. We have high interest in bringing our product to market and think it could be sold for \$50 per kit.

In 24 hours, we created the hardware kit, 3d instructions, web app to click to code the hardware, api to store the programs created from the web app, program interpreter code running on the hardware, and a commercial.

We also won the Best Embedded Wearable Hack (Qualcomm) challenge at Technica 2019!

## Kit

- includes all hardware components to make one pair of sneakers
- components are pre-soldered so all they require is assembling
- contains a QR code link to 3D assembly instructions
- priced at \$50, yet could reduce in bulk
- hardware: Adafruit Feather, neopixel light strands, button, accelerometer, force sensor, sound sensor, UV sensor, air quality sensor
- asset link: https://github.com/itisaasta/lumin8it/tree/master/lumin8it-kit

## Web App

- enables children to click to configure their hardware and see the code as a result
- contains 3D hardware setup instructions to ease their hardware experience
- technology: React, React Bootstrap, Fyuse; hosted on AWS S3 fronted by CloudFront for performance and scalability
- code link: https://github.com/itisaasta/lumin8it/tree/master/lumin8it-web

## API

- stores and retrieves programs created through the web app
- technology: AWS API Gateway, Node.js hosted on AWS Lambda, AWS DynamoDB
- code link: https://github.com/itisaasta/lumin8it/tree/master/lumin8it-api

## Hardware

- connects to wifi, calls API, interprets the programs and runs their functionality
- technology: Arduino (C++)
- code link: https://github.com/itisaasta/lumin8it/tree/master/lumin8it-hardware
