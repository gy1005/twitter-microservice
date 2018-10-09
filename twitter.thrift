namespace cpp twitter
namespace py twitter

struct Tweet_ {
    1: string tweet_id
    2: string user_id
    3: string file_id
    4: string text
}

struct User_ {
    1: string user_id
    2: string username
    3: string homepage
}

struct File_ {
    1: string file_id
    2: string content
}

struct Tweet {
    1: string tweet_id
    2: User_ user
    3: string text
    4: File_ file
}


service UserService {
    User_ getUser_(1: string user_id)
}

service TweetService {
    Tweet_ getTweet_(1: string tweet_id)
}

service FileService {
    File_ getFile_(1: string file_id)
}

service ComposeService {
    Tweet getTweet(1: string user_id, 2: string tweet_id)
}