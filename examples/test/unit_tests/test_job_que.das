// options log

require daslib/jobque_boost

[export]
def test
    with_job_que <|
        with_job_status(5) <| $ ( status )
            for x in range(5)
                var y <- [{for t in range(3); t+x}]
                new_job <| @ [[<-y]]
                    assert(length(y)==3)
                    for t,c in range(3),y
                        assert(t+x==c)
                    status |> notify
            status |> join
    return true
