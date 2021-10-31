FROM python:3.9-slim
WORKDIR /app
VOLUME /app

# use pre-compiled binaries for arm architectures only
# https://www.piwheels.org/project/numpy/
RUN if [ $(arch | cut -c 1-3) = "arm" ] ; then \
        apt-get update && \
        apt-get install -y libatlas3-base libgfortran5 && \
        rm -rf /var/lib/apt/lists/* && \
        echo "[global]\nextra-index-url=https://www.piwheels.org/simple" >> /etc/pip.conf ; \
    fi

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY src/*.py src/

CMD [ "python", "-m", "src.main" ]
