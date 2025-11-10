from glob import glob

from setuptools import find_packages, setup

package_name = 'async_service_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (f'share/{package_name}/launch', glob('launch/*.launch.*')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='SuperJappie08',
    maintainer_email='36795178+SuperJappie08@users.noreply.github.com',
    description='Async service responses in Python without multithreading,'
                ' allows for async service providers',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            f'async_service_relay = {package_name}.async_service_relay:main',
            f'add_three_ints_server = {package_name}.add_three_ints_server:main',
        ],
    },
)
