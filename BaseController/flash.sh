#/bin/bash

st-flash erase

st-flash --format binary write build/BaseController.bin 0x08000000