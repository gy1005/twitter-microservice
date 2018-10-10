namespace cpp twitter
namespace py twitter

struct Timestamp {
    1: string service_name
    2: string function_name
    3: i64 timestamp
}


struct Tweet_ {
    1: string tweet_id
    2: string user_id
    3: string file_id
    4: string text
    5: list<Timestamp> timestamps
}

struct User_ {
    1: string user_id
    2: string username
    3: string homepage
    4: list<Timestamp> timestamps
}

struct File_ {
    1: string file_id
    2: string content
    3: list<Timestamp> timestamps
}

struct Tweet {
    1: string tweet_id
    2: User_ user
    3: string text
    4: File_ file
    5: list<Timestamp> timestamps
}

service UserService {
    User_ getUser_(1: string user_id, 2: list<Timestamp> timestamps)
}

service TweetService {
    Tweet_ getTweet_(1: string tweet_id, 2: list<Timestamp> timestamps)
}

service FileService {
    File_ getFile_(1: string file_id, 2: list<Timestamp> timestamps)
}

service ComposeService {
    Tweet getTweet(1: string user_id, 2: string tweet_id, 3: list<Timestamp> timestamps)
}