FROM python:3.10-slim
WORKDIR /app
VOLUME /app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY src/*.py src/

CMD [ "python", "-m", "src.main" ]
