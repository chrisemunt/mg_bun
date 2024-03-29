# mg_bun

High speed Synchronous and Asynchronous access to InterSystems Cache/IRIS and YottaDB from the Bun JavaScript Engine.

Chris Munt <cmunt@mgateway.com>  
23 June 2023, MGateway Ltd [http://www.mgateway.com](http://www.mgateway.com)

* Two connectivity models to the InterSystems or YottaDB database are provided: High performance via the local database API or network based.
* The status of this module is very much 'work in progress' - use with care!  In the fullness of time, it is envisaged that **mg\_bun** will contain the same functionality as, and be compatible with, **mg-dbx** (our addon for Node.js).
* [Release Notes](#RelNotes) can be found at the end of this document.

Contents

* [Pre-requisites](#PreReq") 
* [Installing mg_bun](#Install)
* [Connecting to the database](#Connect)
* [Invocation of database commands](#DBCommands)
* [Invocation of database functions](#DBFunctions)
* [Transaction Processing](#TProcessing)
* [Direct access to InterSystems classes (IRIS and Cache)](#DBClasses)
* [License](#License)

## <a name="PreReq"></a> Pre-requisites 

**mg\_bun** is an addon for Bun written in C, designed to work through the Bun Foreign Function Interface (FFI).  **mg\_bun** is distributed as C source code and the installation procedure will expect a C compiler to be present on the target system.

Linux systems can use the freely available GNU C compiler (gcc) which can be installed as follows.

Ubuntu:

       apt-get install gcc

Red Hat and CentOS:

       yum install gcc

Apple OS X can use the freely available **Xcode** development environment.

## <a name="Install"></a> Installing mg_bun

Unpack the GitHub distribution.

Assuming that Bun is already installed and a C compiler is available to the installation process:

Building from source on UNIX systems (working in the distribution /src directory):

       make

The shared object **mg\_bun.so** will be created.

### Installing the M support routines (also known as the DB Superserver)

The M support routines are required for:

* Network based access to databases.
* (*for the future*) Direct access to SQL (either via the API or via the network).
* The Merge command under YottaDB (either via the API or via the network).

Two M routines need to be installed (%zmgsi and %zmgsis).  These can be found in the *Service Integration Gateway* (**mgsi**) GitHub source code repository ([https://github.com/chrisemunt/mgsi](https://github.com/chrisemunt/mgsi)).  Note that it is not necessary to install the whole *Service Integration Gateway*, just the two M routines held in that repository.

#### Installation for InterSystems Cache/IRIS

Log in to the %SYS Namespace and install the **zmgsi** routines held in **/isc/zmgsi\_isc.ro**.

       do $system.OBJ.Load("/isc/zmgsi_isc.ro","ck")

Change to your development UCI and check the installation:

       do ^%zmgsi

       MGateway Ltd - Service Integration Gateway
       Version: 4.5; Revision 28 (3 February 2023)


#### Installation for YottaDB

The instructions given here assume a standard 'out of the box' installation of **YottaDB** (version 1.30) deployed in the following location:

       /usr/local/lib/yottadb/r130

The primary default location for routines:

       /root/.yottadb/r1.30_x86_64/r

Copy all the routines (i.e. all files with an 'm' extension) held in the GitHub **/yottadb** directory to:

       /root/.yottadb/r1.30_x86_64/r

Change directory to the following location and start a **YottaDB** command shell:

       cd /usr/local/lib/yottadb/r130
       ./ydb

Link all the **zmgsi** routines and check the installation:

       do ylink^%zmgsi

       do ^%zmgsi

       MGateway Ltd - Service Integration Gateway
       Version: 4.5; Revision 28 (3 February 2023)

Note that the version of **zmgsi** is successfully displayed.

Finally, add the following lines to the interface file (**zmgsi.ci** in the example used in the db.open() method).

       sqlemg: ydb_string_t * sqlemg^%zmgsis(I:ydb_string_t*, I:ydb_string_t *, I:ydb_string_t *)
       sqlrow: ydb_string_t * sqlrow^%zmgsis(I:ydb_string_t*, I:ydb_string_t *, I:ydb_string_t *)
       sqldel: ydb_string_t * sqldel^%zmgsis(I:ydb_string_t*, I:ydb_string_t *)
       ifc_zmgsis: ydb_string_t * ifc^%zmgsis(I:ydb_string_t*, I:ydb_string_t *, I:ydb_string_t*)

A copy of this file can be downloaded from the **/unix** directory of the  **mgsi** GitHub repository [here](https://github.com/chrisemunt/mgsi)

### Starting the DB Superserver (for network based connectivity only)

The default TCP server port for **zmgsi** is **7041**.  If you wish to use an alternative port then modify the following instructions accordingly.

* For InterSystems DB servers the concurrent TCP service should be started in the **%SYS** Namespace.

Start the DB Superserver using the following command:

       do start^%zmgsi(0) 

To use a server TCP port other than 7041, specify it in the start-up command (as opposed to using zero to indicate the default port of 7041).

* For YottaDB, as an alternative to starting the DB Superserver from the command prompt, Superserver processes can be started via the **xinetd** daemon.  Instructions for configuring this option can be found in the **mgsi** repository [here](https://github.com/chrisemunt/mgsi)


## <a name="Connect"></a> Connecting to the database

The first step is to import the **mg\_bun** server class to to your Bun script.  The class is implemented in **mg\_bun.ts**.  This class provides the 'glue' to the **mg\_bun** addon - which should also be present in your working directory.

       import {server} from './mg_bun.ts';

And optionally (as required):

       import {mglobal} from './mg_bun.ts';
       import {mclass} from './mg_bun.ts';

### Create a Server Object

       const db = new server();

### Invoking Bun code using mg\_bun

       bun run mybuncode.ts

### Open a connection to the database

In the following examples, modify all paths (and any user names and passwords) to match those of your own installation.

#### InterSystems Cache

##### API based connectivity

Assuming Cache is installed under **/opt/cache20181/**

           db.type = 'Cache';
           db.path = '/opt/cache20181/mgr';
           db.password = '_SYSTEM';
           db.username = 'SYS';
           db.namespace = 'USER';
           ret ret = db.open();

##### Network based connectivity

Assuming Cache is accessed via **localhost** listening on TCP port **7041**

           db.type = 'Cache';
           db.host = 'localhost';
           db.tcp_port = 7041;
           db.password = '_SYSTEM';
           db.username = 'SYS';
           db.namespace = 'USER';
           ret ret = db.open();

#### InterSystems IRIS

##### API based connectivity

Assuming IRIS is installed under **/opt/IRIS20181/**

           db.type = 'IRIS';
           db.path = '/opt/IRIS20181/mgr';
           db.password = '_SYSTEM';
           db.username = 'SYS';
           db.namespace = 'USER';
           ret ret = db.open();

##### Network based connectivity

Assuming IRIS is accessed via **localhost** listening on TCP port **7041**

           db.type = 'IRIS';
           db.host = 'localhost';
           db.tcp_port = 7041;
           db.password = '_SYSTEM';
           db.username = 'SYS';
           db.namespace = 'USER';
           ret ret = db.open();

#### YottaDB

##### API based connectivity

Assuming an 'out of the box' YottaDB installation under **/usr/local/lib/yottadb/r130**.

           var envvars = "";
           envvars = envvars + "ydb_dir=/root/.yottadb\n";
           envvars = envvars + "ydb_rel=r1.30_x86_64\n";
           envvars = envvars + "ydb_gbldir=/root/.yottadb/r1.30_x86_64/g/yottadb.gld\n";
           envvars = envvars + "ydb_routines=/root/.yottadb/r1.30_x86_64/o*(/root/.yottadb/r1.30_x86_64/r /root/.yottadb/r) /usr/local/lib/yottadb/r130/libyottadbutil.so\n";
           envvars = envvars + "ydb_ci=/usr/local/lib/yottadb/r130/zmgsi.ci\n";
           envvars = envvars + "\n";

           db.type = 'YottaDB';
           db.path = '/usr/local/lib/yottadb/r130';
           db.env_vars = envvars;
           ret ret = db.open();

##### Network based connectivity

Assuming YottaDB is accessed via **localhost** listening on TCP port **7041**

           db.type = 'YottaDB';
           db.host = 'localhost';
           db.tcp_port = 7041;
           ret ret = db.open();


#### Additional (optional) properties for the open() method

* **timeout**: The timeout (in seconds) to be applied to database operations invoked via network based connections.  The default value is 10 seconds.


### Return the version of mg_bun

       var result = db.version();

Example:

       console.log("\nmg_bun Version: " + db.version());


### Returning (and optionally changing) the current directory (or Namespace)

       current_namespace = db.namespace([<new_namespace>]);

Example 1 (Get the current Namespace): 

       var nspace = db.namespace();

* Note this will return the current Namespace for InterSystems databases and the value of the current global directory for YottaDB (i.e. $ZG).

Example 2 (Change the current Namespace): 

       var new_nspace = db.namespace("SAMPLES");

* If the operation is successful this method will echo back the new Namespace name.  If not successful, the method will return the name of the current (unchanged) Namespace.


### Close database connection

       db.close();
 

## <a name="DBCommands"></a> Invocation of database commands

### Register a global name (and fixed key)

It is not necessary to create a global object to represent a M global name and fixed key (if any), but this method does give better performance - particularly in cases where the same global reference is repeatedly used.
 
       const global = new mglobal(db, <global_name>[, <fixed_key>]);

Example (using a global named "Person"):

       const person = new mglobal("Person");

### Set a record

_Using a global object:_

       var result = global.set(<key>, <data>);
      
Example:

       person.set(1, "John Smith");

_Alternatively:_

       var result = db.set(<global>, <key>, <data>);
      
Example:

       db.set("person", 1, "John Smith");

### Get a record

_Using a global object:_

       var result = global.get(<key>);
      
Example:

       var name = person.get(1);

_Alternatively:_

       var result = db.get(<global>, <key>);
      
Example:

       var name = db.get("person", 1);

### Delete a record

_Using a global object:_

       var result = global.delete(<key>);
      
Example:

       var result = person.delete(1);

_Alternatively:_

       var result = db.delete(<global>, <key>);
      
Example:

       var result = db.delete("person", 1);


### Check whether a record is defined

_Using a global object:_

       var result = global.defined(<key>);
      
Example:

       var result = person.defined(1);

_Alternatively:_

       var result = db.defined(<global>, <key>);
      
Example:

       var result = db.defined("person", 1);


### Parse a set of records (in order)

_Using a global object:_

       var result = global.next(<key>);
      
Example:

       var key = "";
       while ((key = person.next(key)) != "") {
          console.log("\nPerson: " + key + ' : ' + person.get(key));
       }


_Alternatively:_

       var result = db.next(<global>, <key>);
      
Example:

       var key = "";
       while ((key = db.next("person", key)) != "") {
          console.log("\nPerson: " + key + ' : ' + db.get("person", key));
       }


### Parse a set of records (in reverse order)

_Using a global object:_

       var result = global.previous(<key>);
      
Example:

       var key = "";
       while ((key = person.previous(key)) != "") {
          console.log("\nPerson: " + key + ' : ' + person.get(key));
       }


_Alternatively:_

       var result = db.previous(<global>, <key>);
      
Example:

       var key = "";
       while ((key = db.previous("person", key)) != "") {
          console.log("\nPerson: " + key + ' : ' + db.get("person", key));
       }


### Increment the value of a global node

_Using a global object:_

       var result = global.increment(<key>, <increment_value>);
      
Example (increment the value of the "counter" node by 1.5 and return the new value):

       var result = global.increment("counter", 1.5);

_Alternatively:_

       var result = db.increment(<global>, <key>, <increment_value>);
      
Example (increment the value of the "counter" node by 1.5 and return the new value):

       var result = db.increment("person", "counter", 1.5);


### Lock a global node

_Using a global object:_

       var result = global.lock(<key>, <timeout>);
      
Example (lock global node '1' with a timeout of 30 seconds):

       var result = global.lock(1, 30);

_Alternatively:_

       var result = db.lock(<global>, <key>, <timeout>);
      
Example (lock global node '1' with a timeout of 30 seconds):

       var result = db.lock("person", 1, 30);

* Note: Specify the timeout value as '-1' for no timeout (i.e. wait until the global node becomes available to lock).


### Unlock a (previously locked) global node

_Using a global object:_

       var result = global.unlock(<key>);
      
Example (unlock global node '1'):

       var result = global.unlock(1);

_Alternatively:_

       var result = db.unlock(<global>, <key>);
      
Example (unlock global node '1'):

       var result = db.unlock("person", 1);


### Merge (or copy) part of one global to another

* Note: In order to use the 'Merge' facility with YottaDB the M support routines should be installed (**%zmgsi** and **%zmgsis**).

Global objects must be used to invoke the Merge command.

Merge from global2 to global1:

       var result = <global1>.merge([<key1>,] <global2> [, <key2>]);
      
Example 1 (merge ^MyGlobal2 to ^MyGlobal1):

       global1 = new mglobal(db, 'MyGlobal1');
       global2 = new mglobal(db, 'MyGlobal2');
       global1.merge(global2);

Example 2 (merge ^MyGlobal2(0) to ^MyGlobal1(1)):

       global1 = new mglobal(db, 'MyGlobal1', 1);
       global2 = new mglobal(db, 'MyGlobal2', 0);
       global1.merge(global2);

Alternatively:

       global1 = new mglobal(db, 'MyGlobal1');
       global2 = new mglobal(db, 'MyGlobal2');
       global1.merge(1, global2, 0);


## <a name="DBFunctions"></a> Invocation of database functions

       result = db.function(<function>, <parameters>);
      
Example:

M routine called 'math':

       add(a, b) ; Add two numbers together
                 quit (a+b)

JavaScript invocation:

      result = db.function("add^math", 2, 3);


## <a name="TProcessing"></a> Transaction Processing

M DB Servers implement Transaction Processing by means of the methods described in this section.  When implementing transactions, care should be taken with JavaScript operations that are invoked asynchronously.  All the Transaction Processing methods describe here can only be invoked synchronously.  


### Start a Transaction

       result = db.tstart(<parameters>);

* At this time, this method does not take any arguments.
* On successful completion this method will return zero, or an error code on failure.

Example:

       result = db.tstart();


### Determine the Transaction Level

       result = db.tlevel(<parameters>);

* At this time, this method does not take any arguments.
* Transactions can be nested and this method will return the level of nesting.  If no Transaction is active this method will return zero.  Otherwise a positive integer will be returned to represent the current depth of Transaction nesting.

Example:

       tlevel = db.tlevel();


### Commit a Transaction

       result = db.tcommit(<parameters>);

* At this time, this method does not take any arguments.
* On successful completion this method will return zero, or an error code on failure.

Example:

       result = db.tcommit();


### Rollback a Transaction

       result = db.trollback(<parameters>);

* At this time, this method does not take any arguments.
* On successful completion this method will return zero, or an error code on failure.

Example:

       result = db.trollback();


## <a name="DBClasses"></a> Direct access to InterSystems classes (IRIS and Cache)

### Invocation of a ClassMethod

       const myclass = new mclass(db, <class_name>);
       result = myclass.classmethod(<classmethod_name>, <parameters>);
Or:

       result = db.classmethod(<class_name>, <classmethod_name>, <parameters>);
      
Example (Encode a date to internal storage format):

       result = db.classmethod("%Library.Date", "DisplayToLogical", "10/10/2019");


### Creating and manipulating instances of objects

The following simple class will be used to illustrate this facility.

       Class User.Person Extends %Persistent
       {
          Property Number As %Integer;
          Property Name As %String;
          Property DateOfBirth As %Date;
          Method Age(AtDate As %Integer) As %Integer
          {
             Quit (AtDate - ..DateOfBirth) \ 365.25
          }
       }

### Create an entry for a new Person

       person = db.classmethod("User.Person", "%New");

Add Data:

       result = person.setproperty("Number", 1);
       result = person.setproperty("Name", "John Smith");
       result = person.setproperty("DateOfBirth", "12/8/1995");

Save the object record:

       result = person.method("%Save");

### Retrieve an entry for an existing Person

Retrieve data for object %Id of 1.
 
       person = db.classmethod("User.Person", "%OpenId", 1);

Return properties:

       let number = person.getproperty("Number");
       let name = person.getproperty("Name");
       let dob = person.getproperty("DateOfBirth");

Calculate person's age at a particular date:

       let today = db.classmethod("%Library.Date", "DisplayToLogical", "10/10/2019");
       let age = person.method("Age", today);

### Reusing an object container

Once created, it is possible to reuse containers holding previously instantiated objects using the **reset()** method.  Using this technique helps to reduce memory usage in the Bun environment.

Example 1 Reset a container to hold a new instance:

       person.reset("User.Person", "%New");

Example 2 Reset a container to hold an existing instance (object %Id of 2):

       person.reset("User.Person", "%OpenId", 2);

## <a name="License"></a> License

Copyright (c) 2021-2023 MGateway Ltd,
Surrey UK.                                                      
All rights reserved.
 
http://www.mgateway.com                                                  
Email: cmunt@mgateway.com
 
 
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.      

## <a name="RelNotes"></a>Release Notes

### v1.0.1 (2 August 2022)

* Initial Release

### v1.1.2 (15 June 2022)

* Introduce the **mglobal** and **mclass** classes - designed to be compatible with the equivalent classes in **mg\-dbx**.

### v1.1.3 (20 July 2022)

* Performance improvements.

### v1.1.3a (23 June 2023)

* Documentation update.