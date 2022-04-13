# Redis Cache Data Structure

## Devices and Channels

Specify what devices and channels exist with [SADD](https://redis.io/commands/sadd)

    SADD devices 0 1
    SADD device:0:channels 0 1 2
    SADD device:1:channels 0

Query the stored devices and channels with [SMEMBERS](https://redis.io/commands/smembers)

    SMEMBERS devices
    SMEMBERS device:0:channels
    SMEMBERS device:1:channels

Remove devices or channels with [SREM](https://redis.io/commands/srem)

    SREM devices 0
    SREM device:0:channels 0

## Voltage Measurements

Push multiple measurements with [LPUSH](https://redis.io/commands/lpush) and truncate with [LTRIM](https://redis.io/commands/ltrim).
Note that each measurement consists of a UNIX timestamp (in microseconds) and the value, separated by a `;` character.

    LPUSH device:0:channel:0:voltage 1649879537883432;212.3 1649879537894490;215.21 1649879537904490;210.75
    LTRIM device:0:channel:0:voltage 0 60000

Store or retrive the active sample rate with [SET](https://redis.io/commands/set) and [GET](https://redis.io/commands/get)

    SET device:0:channel:0:sample_rate 100
    GET device:0:channel:0:sample_rate

Pop multiple measurements with [RPOP](https://redis.io/commands/rpop)

    RPOP device:0:channel:0:voltage 100

Check the current number of measurements with [LLEN](https://redis.io/commands/llen)

    LLEN device:0:channel:0:voltage
