#!/usr/bin/env python3
import multiprocessing
import queue
import threading
import argparse
import boto3
import json
import datetime
import time
import logging
import re
from botocore.exceptions import ClientError
import decimal

# Helper class to convert a DynamoDB item to JSON.
class DecimalEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, decimal.Decimal):
            if abs(o) % 1 > 0:
                return float(o)
            else:
                return int(o)
        return super(DecimalEncoder, self).default(o)


boto3_session = boto3.Session()
# client = boto3_session.client(
#     "dynamodb",
#     aws_access_key_id="AKIAIQRY3OMUH5EM4YXA",
#     aws_secret_access_key="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",
#     region_name="us-west-2"
# )
# table = dynamodb.Table('f1_2018')

client = boto3.resource('dynamodb',
                          aws_access_key_id="AKIAIQRY3OMUH5EM4YXA",
                          aws_secret_access_key="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",
                          region_name='us-west-2')

# table = client.Table('f1_2018')
with open("../silverstone.json") as json_file:
    telemetry = json.load(json_file, parse_float=decimal.Decimal)
    for t in telemetry:
        try:

            if 'dummy' in t or t['table'] not in ['telemetry_data', 'lap_data', 'motion_data', 'car_setup', 'session_data']:
                continue

            table = client.Table(t['table'])
            response = table.put_item(
                Item=t
            )
        except KeyError as ke:
            print(json.dumps(t, indent=4, cls=DecimalEncoder))

