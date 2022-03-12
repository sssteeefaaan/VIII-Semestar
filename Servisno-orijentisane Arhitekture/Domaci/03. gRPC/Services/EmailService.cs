using Grpc.Core;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace _03._gRPC
{
    public class EmailService : Email.EmailBase
    {
        private readonly ILogger<EmailService> _logger;
        private static Dictionary<String, User> _users = null;
        private Dictionary<String, User> Users
        {
            get
            {
                if (_users == null)
                    _users = new Dictionary<string, User>();
                return _users;
            }
        }
        public EmailService(ILogger<EmailService> logger)
        {
            _logger = logger;
        }

        public override Task<Response> Register(Credentials credentials, ServerCallContext context)
        {
            Response ret = new Response();
            if (credentials == null || String.IsNullOrEmpty(credentials.Email) || String.IsNullOrEmpty(credentials.Password))
            {
                ret.Code = 401;
                ret.Message = "Incorrect email or password!";
            }
            else if (Users.ContainsKey(credentials.Email))
            {
                ret.Code = 401;
                ret.Message = "Email already registered!";
            }
            else {
                Users.Add(credentials.Email, new User()
                {
                    Credentials = credentials,
                    Unread = new List<Content>()
                });
                ret.Code = 200;
                ret.Message = "User successfully registered!";
            }

            return Task.FromResult(ret);
        }

        public override Task<Response> SendEmail(Content email, ServerCallContext context)
        {
            Response ret = new Response();

            if (!Users.ContainsKey(email.FromUser))
            {
                ret.Code = 401;
                ret.Message = "Sender not registered!";
            }
            else if (!Users.ContainsKey(email.ToUser))
            {
                ret.Code = 401;
                ret.Message = "Receiver not registered!";
            }
            else
            {
                Users[email.ToUser].Unread.Add(email);
                ret.Code = 200;
                ret.Message = "Email sent successfully!";
            }

            return Task.FromResult(ret);
        }

        public override async Task CheckMailbox(Credentials credentials, IServerStreamWriter<Content> responseStream, ServerCallContext context)
        {
            if (credentials != null && !String.IsNullOrEmpty(credentials.Email) && !String.IsNullOrEmpty(credentials.Password))
            {
                User u = null;
                if (Users.TryGetValue(credentials.Email, out u) && u.Credentials.Password == credentials.Password)
                {
                    foreach (Content unread in u.Unread)
                        await responseStream.WriteAsync(unread);
                    u.Unread.Clear();
                }
            }
        }
    }
}
