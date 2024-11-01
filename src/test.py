import asyncio
from bleak import BleakClient, BleakScanner

# 全局变量
target_name = "ESP32_Laptop_Server"  # ESP32的设备名，需要与ESP32代码中的名称一致
target_address = None

# UUID 用于找到 BLE 设备的服务和特征，请根据实际设备调整
CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"  # 示例特征 UUID

async def scan_for_device(target_name):
    """
    扫描可用蓝牙设备，并返回目标设备地址
    """
    print("Scanning for available Bluetooth devices...")
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"Found: {device.name} - {device.address}")
        if device.name == target_name:
            print(f"Found target Bluetooth device {device.name} with address {device.address}")
            return device.address
    return None

async def connect_and_communicate(target_address):
    """
    连接到指定蓝牙设备，并进行消息收发
    """
    async with BleakClient(target_address) as client:
        try:
            # 确保已连接
            if await client.is_connected():
                print("Successfully connected to ESP32")

                # 主循环进行消息收发
                while True:
                    # 发送消息
                    message_to_send = "Hello from Laptop!"
                    await client.write_gatt_char(CHARACTERISTIC_UUID, message_to_send.encode())
                    print(f"Sent: {message_to_send}")
                    await asyncio.sleep(2)

                    # 接收消息
                    data = await client.read_gatt_char(CHARACTERISTIC_UUID)
                    if data:
                        message = data.decode().strip()
                        print(f"Received: {message}")

                        # 调用消息处理函数
                        handle_received_message(message)
        except Exception as err:
            print(f"Bluetooth communication error: {err}")

def handle_received_message(message):
    """
    处理从ESP32接收到的消息
    """
    print(f"Handling received message: {message}")
    
    if "special command" in message:
        print("Received special command! Executing special response...")

    elif "turn on light" in message:
        print("Turning on the light...")

    elif "turn off light" in message:
        print("Turning off the light...")

    else:
        print("Unrecognized command received. No action taken.")

async def main():
    # 扫描目标设备
    target_address = await scan_for_device(target_name)
    if target_address is None:
        print("Could not find target Bluetooth device")
        return

    # 连接目标设备并进行通信
    await connect_and_communicate(target_address)

# 启动程序
if __name__ == "__main__":
    asyncio.run(main())
