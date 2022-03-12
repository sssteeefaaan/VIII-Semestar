using System.Collections.Generic;

namespace _03._gRPC
{
    public class User
    {
        public Credentials Credentials { get; set; }
        public List<Content> Unread { get; set; }
    }
}