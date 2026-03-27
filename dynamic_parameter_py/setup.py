from setuptools import find_packages, setup

package_name = 'dynamic_parameter_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    package_data={'': ['py.typed']},
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='SuperJappie08',
    maintainer_email='36795178+SuperJappie08@users.noreply.github.com',
    description='Dynamically typed parameter demos for rclpy',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'dynamic_parameter_talker = dynamic_parameter_py.dynamic_parameter_talker:main'
        ],
    },
)
