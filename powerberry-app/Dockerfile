FROM python:3.10-slim

WORKDIR /app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY powerberry/*.py powerberry/

CMD [ "python", "-m", "powerberry" ]
