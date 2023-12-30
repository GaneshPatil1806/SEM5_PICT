import subprocess
host = '10.10.13.191'
user = 'pict'
cmd = 'sudo apt-get install wireshark -y'
try:
    sshCmd = f'ssh {user}@{host} -S pict@123 "{cmd}"'
    subprocess.check_call(sshCmd, shell=True)
    print("wireshark has been successfully installed on the remote system.")
except subprocess.CalledProcessError as e:
    print(f"Installation failed with error code {e.returncode}.\nError message: {e.output}")
except Exception as e:
    print(f"An error occurred: {str(e)}")