# Dockerfile build. This uses the environment build as a base and will setup
FROM myusername/mplab-env:latest  # Use the base environment image

# Copy project code
WORKDIR /Gen-3
COPY Firmware/ Firmware/

# Run build script
#CMD ["make"]