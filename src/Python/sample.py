import json
import pathlib

from asst import Asst, Message


@Asst.CallBackType
def my_callback(msg, details, arg):
    m = Message(msg)
    d = json.loads(details.decode('utf-8'))

    print(m, d, arg)


if __name__ == "__main__":

    # 请设置为存放 dll 文件及资源的路径
    path = pathlib.Path.cwd().parent

    Asst.load(path=path)

    # 若需要获取详细执行信息，请传入 callback 参数
    # 例如 asst = Asst(callback=my_callback)
    asst = Asst()

    # 请自行配置 adb 环境变量，或修改为 adb 可执行程序的路径
    if asst.connect('adb.exe', '127.0.0.1:5555'):
        print('连接成功')
    else:
        print('连接失败')
        exit()

    # 任务及参数请参考 docs/集成文档.md

    asst.append_task('StartUp')
    asst.append_task('Fight', {
        'stage': 'LastBattle',
        'report_to_penguin': True,
        # 'penguin_id': '1234567'
    })
    asst.append_task('Recruit', {
        'select': [4],
        'confirm': [3, 4],
        'times': 4
    })
    asst.append_task('Infrast', {
        'facility': [
            "Mfg", "Trade", "Control", "Power", "Reception", "Office", "Dorm"
        ],
        'drones': "Money"
    })
    asst.append_task('Visit')
    asst.append_task('Mall', {
        'shopping': True,
        'buy_first': ['招聘许可', '龙门币'],
        'blacklist': ['家具', '碳'],
    })
    asst.append_task('Award')
    # asst.append_task('Copilot', {
    #     'stage_name': '千层蛋糕',
    #     'filename': './GA-EX8-raid.json',
    #     'formation': False
    # })

    asst.start()

    while True:
        input('>')
