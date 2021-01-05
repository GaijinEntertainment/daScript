// options log

require jobque
require daslib/jobque_boost

[export]
def test
    with_job_que <|
        with_job_status(5) <| $ ( status )
            for x in range(5)
                var y <- [{for t in range(3); t+x}]
                pfork <| @ [[<-y]]
                    print("x = {x}, y = {y}, context = {this_context()}\n")
                    status |> notify
            status |> join
    return true
