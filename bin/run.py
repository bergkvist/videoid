import os

asset_id = 'ZTHsrEG5jhA'
video_ids = [
    'M_KWGJw6R24', 
    'ZTHsrEG5jhA', 
    'j4hnIAqyhGE', 
    '9RlBXx8Pf-8',
    'B9ypGdx5EXA',
    'WLrbqsXwKz4',
    '8b1lO5NuaEs'
]

for video_id in video_ids:
    os.system("./main.exe " + asset_id + " " + video_id)