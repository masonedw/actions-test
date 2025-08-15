# FROM ubuntu:22.04
# ENV DEBIAN_FRONTEND=noninteractive

# RUN apt-get update && apt-get install -y wget unzip make bash libxext6 curl libxrender1 libxtst6 libxi6 libfreetype6 openjdk-11-jre-headless && rm -rf /var/lib/apt/lists/*

# WORKDIR /tmp

# RUN curl -fSL -A "Mozilla/4.0" -o /tmp/xc32.run "https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc32-v4.60-full-install-linux-x64-installer.run" \
#     && chmod a+x /tmp/xc32.run \
#     && /tmp/xc32.run --mode unattended --unattendedmodeui none \
#         --netservername localhost --LicenseType FreeMode --prefix /opt/microchip/xc32 \
#     && rm /tmp/xc32.run

# RUN curl -fSL -A "Mozilla/4.0" -o /tmp/mplabx-installer.tar "http://ww1.microchip.com/downloads/en/DeviceDoc/MPLABX-v5.45-linux-installer.tar"  && tar xf /tmp/mplabx-installer.tar && rm /tmp/mplabx-installer.tar  && USER=root ./*-installer.sh --nox11     -- --unattendedmodeui none --mode unattended  && rm ./*-installer.sh

# WORKDIR /opt/microchip/packs
# RUN wget https://packs.download.microchip.com/Microchip.SAME54_DFP.3.10.249.atpack \
#     && unzip Microchip.SAME54_DFP.3.10.249.atpack -d SAME54_DFP \
#     && rm Microchip.SAME54_DFP.3.10.249.atpack

# ENV PATH="/opt/microchip/xc32/bin:$PATH"
# ENV PATH="/opt/microchip/mplabx/mplab_ide/v5.45/bin:$PATH"
# ENV MICROCHIP_PACKS_PATH=/opt/microchip/packs/SAME54_DFP


# WORKDIR /project
# CMD ["bash"]

# Dockerfile environment. This sets up the environemt with all the dependencies needed to run an MPLAB xc32 project for the SAME54 chip 
# =========================
# 1) Builder stage
# =========================
FROM ubuntu:22.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies for installation & running
RUN apt-get update && apt-get install -y \
    wget unzip make bash curl libxext6 libxrender1 libxtst6 libxi6 libfreetype6 openjdk-11-jre-headless \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

# --- Install XC32 ---
RUN curl -fSL -A "Mozilla/4.0" \
    -o xc32.run \
    "https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc32-v4.60-full-install-linux-x64-installer.run" \
 && chmod a+x xc32.run \
 && ./xc32.run --mode unattended --unattendedmodeui none \
        --netservername localhost --LicenseType FreeMode --prefix /opt/microchip/xc32 \
 && rm xc32.run

# --- Install MPLABX ---
RUN curl -fSL -A "Mozilla/4.0" \
    -o mplabx.tar \
    "http://ww1.microchip.com/downloads/en/DeviceDoc/MPLABX-v5.45-linux-installer.tar" \
 && tar xf mplabx.tar \
 && rm mplabx.tar \
 && USER=root ./*-installer.sh --nox11 \
      -- --unattendedmodeui none --mode unattended \
 && rm ./*-installer.sh

# --- Install device pack ---
WORKDIR /opt/microchip/packs
RUN wget https://packs.download.microchip.com/Microchip.SAME54_DFP.3.10.249.atpack \
    && unzip Microchip.SAME54_DFP.3.10.249.atpack -d SAME54_DFP \
    && rm Microchip.SAME54_DFP.3.10.249.atpack

# Optional: strip debug symbols to save space
RUN find /opt/microchip -type f -name '*.a' -o -name '*.o' | xargs strip --strip-unneeded 2>/dev/null || true

# Optional: remove docs, examples, or other large unused dirs
RUN rm -rf /opt/microchip/mplabx/docs \
           /opt/microchip/mplabx/examples \
           /opt/microchip/xc32/docs \
           /opt/microchip/xc32/*/lib/*.a

# =========================
# 2) Runtime stage
# =========================
FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# Install only runtime dependencies (much smaller)
RUN apt-get update && apt-get install -y \
    bash make libxext6 libxrender1 libxtst6 libxi6 libfreetype6 \
    && rm -rf /var/lib/apt/lists/*

# Bulk copy of microchip 14.6GB of stuff. 
#COPY --from=builder /opt/microchip /opt/microchip

# Only copy the makefile regenerator files from the MPLABX install from the build stage
COPY --from=builder /opt/microchip/mplabx/v5.45/mplab_platform/bin/prjMakefilesGenerator.sh opt/microchip/mplabx/v5.45/mplab_platform/bin/prjMakefilesGenerator.sh
COPY --from=builder /opt/microchip/mplabx/v5.45/mplab_platform/lib/PrjMakefilesGenerator.jar /opt/microchip/mplabx/v5.45/mplab_platform/lib/PrjMakefilesGenerator.jar
COPY --from=builder /opt/microchip/mplabx/v5.45/mplab_platform/etc/mplab_ide.conf /opt/microchip/mplabx/v5.45/mplab_platform/etc/mplab_ide.conf
COPY --from=builder /opt/microchip/mplabx/v5.45/mplab_platform/lib /opt/microchip/mplabx/v5.45/mplab_platform/lib
COPY --from=builder /opt/microchip/mplabx/v5.45/sys/java /opt/microchip/mplabx/v5.45/sys/java
COPY --from=builder /opt/microchip/mplabx/v5.45/mplab_platform /opt/microchip/mplabx/v5.45/mplab_platform
COPY --from=builder /opt/microchip/mplabx/v5.45/packs/Microchip/SAME54_DFP /opt/microchip/mplabx/v5.45/packs/Microchip/SAME54_DFP

# Copy SAME54_DFP
COPY --from=builder /opt/microchip/packs /opt/microchip/packs

# Copy XC32 compiler binaries
COPY --from=builder /opt/microchip/xc32/bin /opt/microchip/xc32/bin
COPY --from=builder /opt/microchip/xc32/etc /opt/microchip/xc32/etc
COPY --from=builder /opt/microchip/xc32/lib /opt/microchip/xc32/lib
COPY --from=builder /opt/microchip/xc32/lib /opt/microchip/xc32/lib
COPY --from=builder /opt/microchip/xc32/pic32c /opt/microchip/xc32/pic32c


# Set environment variables
ENV PATH="/opt/microchip/xc32/bin:$PATH"
ENV PATH="/opt/microchip/mplabx/mplab_ide/v5.45/bin:$PATH"
ENV MICROCHIP_PACKS_PATH=/opt/microchip/packs/SAME54_DFP

WORKDIR /project
CMD ["bash"]


#/opt/microchip/mplabx/v5.45/mplab_platform/bin/prjMakefilesGenerator.sh
#/opt/microchip/mplabx/mplab_ide/v5.45/lib/PrjMakefilesGenerator.jar
#/opt/microchip/mplabx/v5.45/mplab_platform/etc/mplab_ide.conf
#/opt/microchip/mplabx/v5.45/mplab_platform/lib