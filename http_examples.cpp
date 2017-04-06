#include "server_http.hpp"
#include "client_http.hpp"

//Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Added for the default_resource example
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

//db
#include "../../Database/Database.h"
#include "../../Util/Util.h"

using namespace std;
//Added for the json-example:
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

//Added for the default_resource example
void default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                           const shared_ptr<ifstream> &ifs);

Database *current_database = NULL;

int main() {
    //HTTP-server at port 8080 using 1 thread
    //Unless you do more heavy non-threaded processing in the resources,
    //1 thread is usually faster than several threads
    HttpServer server;
    server.config.port=8080;

    //GET-example for the path /build/[db_name]/[db_path], responds with the matched string in path
    //For instance a request GET /build/db/123 will receive: db 123
    //server.resource["^/build/([a-zA-Z]+[0-9]*)/([a-zA-Z]+/*[a-zA-Z]+[0-9]*.n[a-zA-Z]*[0-9]*)$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    server.resource["^/build/*/*$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        string db_name=request->path_match[1];
	      string db_path=request->path_match[2];
        if(db_name=="" || db_path=="")
        {
          string error = "Exactly 2 arguments required!";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }

        string database = db_name;
        if(database.length() > 3 && database.substr(database.length()-3, 3) == ".db")
        {
          string error = "Your db name to be built should not end with \".db\".";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }
        database += ".db";

        string dataset = db_path;

        if(current_database != NULL)
        {
          string error = "Please unload your database first.";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }
        current_database = new Database(database);
        bool flag = current_database->build(dataset);
        delete current_database;
        current_database = NULL;
        if(!flag)
        {
          string error = "Import RDF file to database failed.";
          string cmd = "rm -r " + database;
          system(cmd.c_str());
          return -1;
        }

        string success = "Import RDF file to database done.";
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << success.length() << "\r\n\r\n" << success;
        return 0;
    };

    //GET-example for the path /load/[db_name], responds with the matched string in path
    //For instance a request GET /load/db123 will receive: db123
    server.resource["^/load/*$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        string db_name=request->path_match[1];

        if(db_name=="")
        {
          string error = "Exactly 1 argument is required!";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }

        string database = db_name;
        if(database.length() > 3 && database.substr(database.length()-3, 3) == ".db")
        {
          string error = "Your db name to be built should not end with \".db\".";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }
        if(current_database != NULL)
        {
          string error = "Please unload your current database first.";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }
        current_database = new Database(database);
        bool flag = current_database->load();
        if (!flag)
        {
          string error = "Failed to load the database.";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          delete current_database;
          current_database = NULL;
          return -1;
        }
        string success = "Database loaded successfully.";
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << success.length() << "\r\n\r\n" << success;

        return 0
    };

    //GET-example for the path /query/[query_file_path], responds with the matched string in path
    //For instance a request GET /query/db123 will receive: db123
    server.resource["^/query/*$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        string db_name=request->path_match[1];
        string db_path=request->path_match[2];
        string str = db_name;

        if(current_database == NULL)
        {
          string error = "No database in use!";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        };

        string sparql;

        if(db_path[0]=='\"')
        {
          sparql = db_path[0].substr(1, db_path.length()-2);
        }
        else
        {
          string ret = Util::getExactPath(db_path[0].c_str());
          const char *path = ret.c_str();
          if(path == NULL)
          {
            string error = "Invalid path of query.";
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
            return -1;
          }
          sparql = Util::getQueryFromFile(path);
        }

        if (sparql.empty()) {
      		cerr << "Empty SPARQL." << endl;
      		return -1;
      	}

        ResultSet rs;
        bool ret = current_database->query(sparql, rs, output);
        if(ret)
        {
          string success = rs.to_str();
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << success.length() << "\r\n\r\n" << success;
          return 0;
        }
        else
        {
          string error = "query() returns false.";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }
    };

    //GET-example for the path /unload/[db_name], responds with the matched string in path
    //For instance a request GET /unload/db123 will receive: db123
    server.resource["^/unload$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        if(current_database == NULL)
        {
          string error = "No database used now.";
          *response << "HTTP/1.1 200 OK\r\nContent-Length: " << error.length() << "\r\n\r\n" << error;
          return -1;
        }

	      delete current_database;
	      current_database = NULL;
        string success = "Database unloaded.";
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << success.length() << "\r\n\r\n" << success;
        return 0;
    };






    server.resource["^/json$"]["POST"]=[](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try {
            ptree pt;
            read_json(request->content, pt);

            string name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");

            *response << "HTTP/1.1 200 OK\r\n"
                      << "Content-Type: application/json\r\n"
                      << "Content-Length: " << name.length() << "\r\n\r\n"
                      << name;
        }
        catch(exception& e) {
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };

    //GET-example for the path /info
    //Responds with request-information
    server.resource["^/info$"]["GET"]=[](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for(auto& header: request->header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }

        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);

        *response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };

    //Get example simulating heavy work in a separate thread
    server.resource["^/work$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            this_thread::sleep_for(chrono::seconds(5));
            string message="Work done";
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
        });
        work_thread.detach();
    };

    //Default GET-example. If no other matches, this anonymous function will be called.
    //Will respond with content in the web/-directory, and its subdirectories.
    //Default file: index.html
    //Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    server.default_resource["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try {
            auto web_root_path=boost::filesystem::canonical("web");
            auto path=boost::filesystem::canonical(web_root_path/request->path);
            //Check if path is within web_root_path
            if(distance(web_root_path.begin(), web_root_path.end())>distance(path.begin(), path.end()) ||
               !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
                throw invalid_argument("path must be within root path");
            if(boost::filesystem::is_directory(path))
                path/="index.html";
            if(!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
                throw invalid_argument("file does not exist");

            std::string cache_control, etag;

            // Uncomment the following line to enable Cache-Control
            // cache_control="Cache-Control: max-age=86400\r\n";

#ifdef HAVE_OPENSSL
            // Uncomment the following lines to enable ETag
            // {
            //     ifstream ifs(path.string(), ifstream::in | ios::binary);
            //     if(ifs) {
            //         auto hash=SimpleWeb::Crypto::to_hex_string(SimpleWeb::Crypto::md5(ifs));
            //         etag = "ETag: \""+hash+"\"\r\n";
            //         auto it=request->header.find("If-None-Match");
            //         if(it!=request->header.end()) {
            //             if(!it->second.empty() && it->second.compare(1, hash.size(), hash)==0) {
            //                 *response << "HTTP/1.1 304 Not Modified\r\n" << cache_control << etag << "\r\n\r\n";
            //                 return;
            //             }
            //         }
            //     }
            //     else
            //         throw invalid_argument("could not read file");
            // }
#endif

            auto ifs=make_shared<ifstream>();
            ifs->open(path.string(), ifstream::in | ios::binary | ios::ate);

            if(*ifs) {
                auto length=ifs->tellg();
                ifs->seekg(0, ios::beg);

                *response << "HTTP/1.1 200 OK\r\n" << cache_control << etag << "Content-Length: " << length << "\r\n\r\n";
                default_resource_send(server, response, ifs);
            }
            else
                throw invalid_argument("could not read file");
        }
        catch(const exception &e) {
            string content="Could not open path "+request->path+": "+e.what();
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
    };

    thread server_thread([&server](){
        //Start server
        server.start();
    });

    //Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    //Client examples
    HttpClient client("localhost:8080");
    auto r1=client.request("GET", "/match/123");
    cout << r1->content.rdbuf() << endl;

    string json_string="{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";
    auto r2=client.request("POST", "/string", json_string);
    cout << r2->content.rdbuf() << endl;

    auto r3=client.request("POST", "/json", json_string);
    cout << r3->content.rdbuf() << endl;

    server_thread.join();

    return 0;
}

void default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                           const shared_ptr<ifstream> &ifs) {
    //read and send 128 KB at a time
    static vector<char> buffer(131072); // Safe when server is running on one thread
    streamsize read_length;
    if((read_length=ifs->read(&buffer[0], buffer.size()).gcount())>0) {
        response->write(&buffer[0], read_length);
        if(read_length==static_cast<streamsize>(buffer.size())) {
            server.send(response, [&server, response, ifs](const boost::system::error_code &ec) {
                if(!ec)
                    default_resource_send(server, response, ifs);
                else
                    cerr << "Connection interrupted" << endl;
            });
        }
    }
}
