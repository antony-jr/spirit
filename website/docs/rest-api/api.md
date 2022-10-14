---
sidebar_position: 2
---

# API Specification

Down below are the different API calls you can make to spirit to do
some supported operations. You **should** obtain the API endpoint first, 
we will assume this url as ```API_URL```.

## Action

**To set or reset, you should do a POST request, use GET request to get the list of
all actions available in the Spirit File.**

Action of the current loaded Spirit File can be modified by using this API endpoint,
the endpoint can be accessed by appending **/action** to the **API_URL**. The request body should 
have json with the following format,

```js
{
   "opt": "set", // or 'unset'
   "action": "some_action" // not need if opt = 'unset'
}
```


```python
#!/usr/bin/env python3
import os
import json
import requests
import tempfile

def get_spirit_api():
    port_file = tempfile.gettempdir() + "/com.github.antony-jr.spirit"
    with open(port_file, "r") as fp:
        port = fp.read()
        p = None
        try:
           p = int(str(port))
        except:
           p = -1

        if p < 0:
            return None
        return "http://127.1:" + str(p) + "/spirit/v1/"

api_url = get_spirit_api()
if api_url is None:
    os.exit(-1)

load = {
    "opt": "set",
    "action": "error",
}

resp = requests.post(api_url + "action", json.dumps(load))
print(resp.content)
```


To get list of all actions available in the Spirit File,

```python
import json
import requests
import tempfile

def get_spirit_api():
    port_file = tempfile.gettempdir() + "/com.github.antony-jr.spirit"
    with open(port_file, "r") as fp:
        port = fp.read()
        p = None
        try:
           p = int(str(port))
        except:
           p = -1

        if p < 0:
            return None
        return "http://127.1:" + str(p) + "/spirit/v1/"

api_url = get_spirit_api()
if api_url is None:
    os.exit(-1)

resp = requests.get(api_url + "action")
print(resp.content)
```


## Property

**HTTP GET request to get current properties of a Spirit File's current action, HTTP POST request to 
update them.**

```
 API_URL/property
``` 

The below api call changes the position of the Spirit currently shown.
```python
#!/usr/bin/env python3
import os
import requests
import tempfile

def get_spirit_api():
    port_file = tempfile.gettempdir() + "/com.github.antony-jr.spirit"
    with open(port_file, "r") as fp:
        port = fp.read()
        p = None
        try:
           p = int(str(port))
        except:
           p = -1

        if p < 0:
            return None
        return "http://127.1:" + str(p) + "/spirit/v1/"

api_url = get_spirit_api()
if api_url is None:
    os.exit(-1)

resp = requests.post(api_url + "property", '{"position": "topLeft"}')
print(resp.content)
```

The following are the possible properties that can be updated,

```js
{
   "speed": 100, // 100 means 100%
   "scale": 40, // 40 means 40% of the original size
   "topXOffset": 0
   "topYOffset": 0,
   "bottomXOffset": 0,
   "bottomYOffset": 0, 
   "position": "topLeft" // topRight, bottomLeft, bottomRight
}
```
