---
sidebar_position: 1
---

# Get API Endpoint.

Spirit REST API is hosted at **localhost** however the port is different on each initialization of
Spirit, one has to get this port number to get the API Endpoint. The port number is stored in a
file stored in the Temporary directory of the operating system, this port can be inactive so the 
programmer has to check if the port is actually hosting Spirit REST API.


The following Python functions gets the API endpoint and request information about the current loaded
"Spirit File".

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

resp = requests.get(api_url + "load")

# Should also check if the request was successful,
# since this file might be from a crash.
print(resp.content)
```
