
from skbuild import setup
import os

setup(
    name = 'qui',
    version = '1.0.0',
    description = "Qt QML bindings",
    long_description = "Bindings for QML part of Qt", 

    url = "https://github.com/mgb4/qui",
    author = "Michael Burisch",
    author_email = "michael.burisch@gmx.com",
    
    keywords = "qt qml python bindings",

    classifiers=[
        "Development Status :: 4 - Beta",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3"
    ],
    
    packages = ["qui"],
	python_requires='>=3',
)

