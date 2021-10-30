FROM python:3.9-slim
# FROM python:3.9-alpine

WORKDIR /app

COPY requirements.txt ./

# install numpy manually on alpine
# RUN apk add --no-cache py3-numpy
# ENV PYTHONPATH /usr/lib/python3.9/site-packages
# RUN sed -i '/numpy/d' requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

COPY powerberry powerberry/

CMD [ "python", "-m", "powerberry" ]
