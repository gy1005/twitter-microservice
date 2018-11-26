namespace cpp twitter
namespace py twitter
namespace lua twitter

struct Tweet_ {
    1: string tweet_id
    2: string user_id
    3: string file_id
    4: string text
    5: string header
}

struct User_ {
    1: string user_id
    2: string username
    3: string homepage
    4: string header
}

struct File_ {
    1: string file_id
    2: string content
    3: string header
}

struct Tweet {
    1: string tweet_id
    2: User_ user
    3: string text
    4: File_ file
    5: string header
}

service UserService {
    User_ getUser_(1: string user_id, 2: string header)
}

service TweetService {
    Tweet_ getTweet_(1: string tweet_id, 2: string header)
}

service FileService {
    File_ getFile_(1: string file_id, 2: string header)
}

service ComposeService {
    Tweet getTweet(1: string user_id, 2: string tweet_id, 3: string header)
}