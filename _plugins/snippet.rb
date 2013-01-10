module Jekyll

  class SnippetBlock < Liquid::Block
    include Liquid::StandardFilters

    # The regular expression syntax checker. Start with the language specifier.
    # Follow that by zero or more space separated options that take one of two
    # forms:
    #
    # 1. name
    # 2. name=value
    SYNTAX = /^([a-zA-Z0-9.+#-]+)((\s+\w+(=\w+)?)*)$/

    def initialize(tag_name, markup, tokens)
      super
      if markup.strip =~ SYNTAX
        @lang = $1
        if defined?($2) && $2 != ''
          tmp_options = {}
          $2.split.each do |opt|
            key, value = opt.split('=')
            if value.nil?
              if key == 'linenos'
                value = 'inline'
              else
                value = true
              end
            end
            tmp_options[key] = value
          end
          tmp_options = tmp_options.to_a.sort.collect { |opt| opt.join('=') }
          # additional options to pass to Albino
          @options = { 'O' => tmp_options.join(',') }
        else
          @options = {}
        end
      else
        raise SyntaxError.new("Syntax Error in 'snippet' - Valid syntax: snippet <lang> [linenos]")
      end
    end

    def parse(tokens)
      @nodelist ||= []
      @nodelist.clear
      
      while token = tokens.shift
        if token =~ FullToken
          if block_delimiter == $1
            end_tag
            return
          end
        end
        @nodelist << token if not token.empty?
      end
    end

    def render(context)
      @options[:encoding] = 'utf-8'

      output = add_code_tags(super, @lang)

      output
    end

    def add_code_tags(code, lang)
      code = '<pre class="syntax '+ lang + '">' + code + '</pre>'

      code
    end

  end

end

Liquid::Template.register_tag('snippet', Jekyll::SnippetBlock)